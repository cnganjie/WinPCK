//////////////////////////////////////////////////////////////////////
// AccessCheck.cpp
// 
//
// �˳����� �����/stsm/liqf ��д
//
// �˴���Ԥ�ƽ��ῪԴ���κλ��ڴ˴�����޸ķ����뱣��ԭ������Ϣ
// 
// 2024.1.1
//////////////////////////////////////////////////////////////////////

#include "AccessCheck.h"
#include <memory>

namespace myMiscUtil {

	BOOL _GetFileSecurity(LPCSTR lpFileName, SECURITY_INFORMATION RequestedInformation, PSECURITY_DESCRIPTOR pSecurityDescriptor, DWORD nLength, LPDWORD lpnLengthNeeded)
	{
		return ::GetFileSecurityA(lpFileName, RequestedInformation, pSecurityDescriptor, nLength, lpnLengthNeeded);
	}

	BOOL _GetFileSecurity(LPCWSTR lpFileName, SECURITY_INFORMATION RequestedInformation, PSECURITY_DESCRIPTOR pSecurityDescriptor, DWORD nLength, LPDWORD lpnLengthNeeded)
	{
		return ::GetFileSecurityW(lpFileName, RequestedInformation, pSecurityDescriptor, nLength, lpnLengthNeeded);
	}
};

// ��Ҫ����Ȩ��GENERIC_XXXXXX���ݸ�dwGenericAccessMask���ɼ���
// �ļ������ļ��е�Ȩ��
template<typename T>
BOOL CanAccessFile(const T* strPath, DWORD dwGenericAccessMask)
{

	DWORD dwSize = 0;
	PSECURITY_DESCRIPTOR psd = NULL;
	SECURITY_INFORMATION si = OWNER_SECURITY_INFORMATION |
		GROUP_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION;

	// ��ȡ�ļ�Ȩ����Ϣ�ṹ���С  
	BOOL bRet = myMiscUtil::_GetFileSecurity(strPath, si, psd, 0, &dwSize);
	if (bRet || ::GetLastError() != ERROR_INSUFFICIENT_BUFFER)
	{
		return FALSE;
	}

	auto pBuf = std::make_unique<char[]>(dwSize);
	ZeroMemory(pBuf.get(), dwSize);
	psd = (PSECURITY_DESCRIPTOR)pBuf.get();

	// ��ȡ�ļ�Ȩ����Ϣ�ṹ���С  
	bRet = myMiscUtil::_GetFileSecurity(strPath, si, psd, dwSize, &dwSize);
	if (!bRet)
	{
		return FALSE;
	}

	HANDLE hToken = NULL;
	if (!::OpenProcessToken(::GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken))
	{
		return FALSE;
	}

	// ģ������  
	HANDLE hImpersonatedToken = NULL;
	if (!::DuplicateToken(hToken, SecurityImpersonation, &hImpersonatedToken))
	{
		::CloseHandle(hToken);
		return FALSE;
	}

	// �ڼ���Ƿ���ĳ��Ȩ��ʱ����GENERIC_WRITE��ֵ�����������ĵڶ�������dwGenericAccessMask
	// GENERIC_WRITE�Ȳ����ڵ���CreateFile�����ļ�ʱ��ʹ�õ����������MapGenericMask��
	// GENERIC_WRITE��ת����FILE_GENERIC_WRITE��
	// ��GENERIC_XXXXXXת����FILE_GENERIC_XXXXXX
	GENERIC_MAPPING genMap{
		.GenericRead = FILE_GENERIC_READ,
		.GenericWrite = FILE_GENERIC_WRITE,
		.GenericExecute = FILE_GENERIC_EXECUTE,
		.GenericAll = FILE_ALL_ACCESS,
	};
	::MapGenericMask(&dwGenericAccessMask, &genMap);

	// ����AccessCheck������Ƿ���ָ����Ȩ��
	PRIVILEGE_SET privileges = { 0 };
	DWORD dwGrantedAccess = 0;
	DWORD privLength = sizeof(privileges);
	BOOL bGrantedAccess = FALSE;
	if (::AccessCheck(psd, hImpersonatedToken, dwGenericAccessMask,
		&genMap, &privileges, &privLength, &dwGrantedAccess, &bGrantedAccess))
	{
		if (bGrantedAccess)
		{
			if (dwGenericAccessMask == dwGrantedAccess)
			{
				bGrantedAccess = TRUE;
			}
			else
			{
				bGrantedAccess = FALSE;
			}
		}
		else
		{
			bGrantedAccess = FALSE;
		}
	}
	else
	{
		bGrantedAccess = FALSE;
	}

	CloseHandle(hImpersonatedToken);
	CloseHandle(hToken);
	return bGrantedAccess;
}

template BOOL CanAccessFile<char>(const char* strPath, DWORD dwGenericAccessMask);
template BOOL CanAccessFile<wchar_t>(const wchar_t* strPath, DWORD dwGenericAccessMask);
