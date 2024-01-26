//////////////////////////////////////////////////////////////////////
// StackWalkerLiqf.cpp
// 
//
// �˳����� �����/stsm/liqf ��д
//
// �˴���Ԥ�ƽ��ῪԴ���κλ��ڴ˴�����޸ķ����뱣��ԭ������Ϣ
// 
// 2024.1.1
//////////////////////////////////////////////////////////////////////


#include "StackWalkerLiqf.h"

StackWalkerLiqf::StackWalkerLiqf(const wchar_t* file)
{
	this->hFile = ::CreateFileW(file, GENERIC_WRITE, 0, 0, OPEN_ALWAYS, 0, 0);
	::SetFilePointer(hFile, 0, 0, FILE_END);
	::GetLocalTime(&tm);

	auto info = std::format(
		"------- ��ջ��Ϣ -----\r\n"
		"����ʱ��        : {:04d}/{:02d}/{:02d} {:02d}:{:02d}:{:02d}\r\n"
		"------- �ļ������ö�ջ -----\r\n",
		tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond);

	DWORD len = info.size();
	::WriteFile(hFile, info.c_str(), info.size(), &len, 0);

}

StackWalkerLiqf::~StackWalkerLiqf()
{
	auto info = std::string("------------------------\r\n\r\n");
	DWORD len = info.size();
	::WriteFile(hFile, info.c_str(), info.size(), &len, 0);
	::CloseHandle(hFile);
}

void StackWalkerLiqf::OnOutput(LPCSTR szText)
{
	DWORD len = strlen(szText);
	::WriteFile(hFile, szText, len, &len, 0);
}

void StackWalkerMem::OnOutput(LPCSTR szText)
{
	this->buffer.append(szText);
}