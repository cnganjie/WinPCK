#pragma once
//////////////////////////////////////////////////////////////////////
// MapViewFile.h: ����ӳ���ļ���ͼ
// 
//
// �˳����� �����/stsm/liqf ��д
//
// �˴���Ԥ�ƽ��ῪԴ���κλ��ڴ˴�����޸ķ����뱣��ԭ������Ϣ
// 
// 2012.4.10
//////////////////////////////////////////////////////////////////////

#define TEST_T 1
/*
#define CREATE_NEW          1
#define CREATE_ALWAYS       2
#define OPEN_EXISTING       3
#define OPEN_ALWAYS         4
#define TRUNCATE_EXISTING   5

#define INVALID_FILE_SIZE ((DWORD)0xFFFFFFFF)
#define INVALID_SET_FILE_POINTER ((DWORD)-1)
#define INVALID_FILE_ATTRIBUTES ((DWORD)-1)
*/

#if _MSC_VER >= 1400
#pragma warning ( disable : 4996 )
#endif

#ifdef _WIN64
typedef unsigned __int64  uintptr_t;
#else
typedef unsigned int uintptr_t;
#endif

#if !defined(_MAPVIEWFILE_H_)
#define _MAPVIEWFILE_H_

typedef unsigned __int64	QWORD;

//LARGE_INTEGER
typedef union _QWORD
{
	QWORD qwValue;
	LONGLONG llwValue;
	struct
	{
		DWORD dwValue;
		DWORD dwValueHigh;
	};
	struct
	{
		LONG lValue;
		LONG lValueHigh;
	};
}UNQWORD, *LPUNQWORD;


class CMapViewFile
{
public:
	CMapViewFile();
	virtual ~CMapViewFile() noexcept;

	BOOL FileExists(const fs::path& szName);

	BOOL	Open(const fs::path& lpszFilename, DWORD dwDesiredAccess, DWORD dwShareMode, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes);

	void	SetFilePointer(QWORD lDistanceToMove, DWORD dwMoveMethod = FILE_BEGIN);
	QWORD	GetFilePointer();

	DWORD	Read(LPVOID buffer, DWORD dwBytesToRead);

	QWORD	GetFileSize();

	virtual LPBYTE	View(QWORD dwAddress, DWORD dwSize) = 0;
	virtual LPBYTE	ReView(LPVOID lpMapAddressOld, QWORD dwAddress, DWORD dwSize) = 0;

	void	UnmapView(void* lpTargetAddress);
	void	UnmapViewAll();
	void	UnMaping();
	void	clear();

	//��ȡ��ǰ�ļ��Ĵ�����
	const char*	GetFileDiskName();

	//ǿ�ƻ���д�����
	virtual BOOL	FlushFileBuffers() = 0;
	
protected:
	////�Զ�����CreateFileMappingAʱ����Ҫ��name
	LPCSTR	GenerateMapName();

	void GetDiskNameFromFilename(const fs::path& lpszFilename);

	uint8_t*	ViewReal(QWORD qwAddress, DWORD dwSize, DWORD dwDesiredAccess);

protected:

	HANDLE	hFile = nullptr;
	HANDLE	hFileMapping = nullptr;
	std::vector<const void*> vMapAddress;


	//�ļ���Ӧ�Ĵ���
	std::string	m_szDisk = "";

	//���ڴ��MapName
	char szFileMappingName[32];

private:
/*
����ΪNTFSϡ���ļ�
����Ϊ��CreateFile->SetSparseFile->CreateFileMapping->MapViewOfFile->UnmapViewOfFile->CloseHandle
*/
	virtual void	SetSparseFile() = 0;
};



class CMapViewFileRead : public CMapViewFile
{
public:
	CMapViewFileRead();
	virtual ~CMapViewFileRead() noexcept;

	BOOL	Open(const fs::path& lpszFilename);

	BOOL	Mapping();

	virtual LPBYTE	View(QWORD dwAddress, DWORD dwSize) override;
	virtual LPBYTE	ReView(LPVOID lpMapAddressOld, QWORD dwAddress, DWORD dwSize) override;

	BOOL	OpenMappingRead(const fs::path& lpFileName);
	LPBYTE	OpenMappingViewAllRead(const fs::path& lpFileName);

	virtual BOOL	FlushFileBuffers() override { return TRUE; };

protected:

private:
	virtual void	SetSparseFile() override {};

};


class CMapViewFileWrite : public CMapViewFile
{
public:
	CMapViewFileWrite();
	virtual ~CMapViewFileWrite() noexcept;

	BOOL	Open(const fs::path& lpszFilename, DWORD dwCreationDisposition, BOOL isNTFSSparseFile = FALSE);

	BOOL	Mapping(QWORD dwMaxSize);

	virtual LPBYTE	View(QWORD dwAddress, DWORD dwSize) override;
	virtual LPBYTE	ReView(LPVOID lpMapAddressOld, QWORD dwAddress, DWORD dwSize) override;

	BOOL	SetEndOfFile();

	DWORD	Write(LPVOID buffer, DWORD dwBytesToWrite);


	BOOL	OpenMappingWrite(const fs::path& lpFileName, DWORD dwCreationDisposition, QWORD qdwSizeToMap);

	virtual BOOL	FlushFileBuffers() override;

private:

	virtual void	SetSparseFile() override;

};

#endif //_MAPVIEWFILE_H_