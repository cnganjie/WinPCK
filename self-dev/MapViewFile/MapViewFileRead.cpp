//////////////////////////////////////////////////////////////////////
// MapViewFileRead.cpp: ����ӳ���ļ���ͼ������
// 
//
// �˳����� �����/stsm/liqf ��д
//
// �˴���Ԥ�ƽ��ῪԴ���κλ��ڴ˴�����޸ķ����뱣��ԭ������Ϣ
// 
// 2014.4.24
//////////////////////////////////////////////////////////////////////
#include "pch_mvf.h"
#include "MapViewFile.h"

CMapViewFileRead::CMapViewFileRead()
{}

CMapViewFileRead::~CMapViewFileRead() noexcept
{}

BOOL CMapViewFileRead::Open(fs::path lpszFilename)
{
	return CMapViewFile::Open(std::ref(lpszFilename), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, OPEN_EXISTING, FILE_FLAG_RANDOM_ACCESS);
}
#if 0
BOOL CMapViewFileRead::Open(LPCWSTR lpszFilename)
{
	return CMapViewFile::Open(lpszFilename, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, OPEN_EXISTING, FILE_FLAG_RANDOM_ACCESS);
}
#endif
BOOL CMapViewFileRead::Mapping()
{
	if(NULL == (this->hFileMapping = ::CreateFileMappingA(this->hFile, NULL, PAGE_READONLY, 0, 0, GenerateMapName()))) {
		assert(FALSE);
		return FALSE;
	}

	return TRUE;
}

LPBYTE CMapViewFileRead::View(QWORD qdwAddress, DWORD dwSize)
{
	return this->ViewReal(qdwAddress, dwSize, FILE_MAP_READ);
}


LPBYTE CMapViewFileRead::ReView(LPVOID lpMapAddressOld, QWORD dwAddress, DWORD dwSize)
{
	this->UnmapView(lpMapAddressOld);
	return this->View(dwAddress, dwSize);
}

BOOL CMapViewFileRead::OpenMappingRead(fs::path lpFileName)
{
	if(!(this->Open(std::ref(lpFileName))))
		return FALSE;

	if(!(this->Mapping()))
		return FALSE;

	return TRUE;
}
#if 0
BOOL CMapViewFileRead::OpenMappingRead(LPCWSTR lpFileName)
{
	if(!(Open(lpFileName)))
		return FALSE;

	if(!(Mapping()))
		return FALSE;

	return TRUE;
}
#endif
LPBYTE CMapViewFileRead::OpenMappingViewAllRead(fs::path lpFileName)
{
	if(this->OpenMappingRead(std::ref(lpFileName)))
		return this->View(0, 0);
	else
		return nullptr;
}
#if 0
LPBYTE CMapViewFileRead::OpenMappingViewAllRead(LPCWSTR lpFileName)
{
	if(OpenMappingRead(lpFileName))
		return View(0, 0);
	else
		return NULL;
}
#endif