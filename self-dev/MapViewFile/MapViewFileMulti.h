#pragma once
#include "MapViewFile.h"

#include <vector>

#ifdef _DEBUG
#define FLUSH_SIZE_THRESHOLD	(320 * 1024 * 1024)
#else
#define FLUSH_SIZE_THRESHOLD	(32 * 1024 * 1024)
#endif

class CMapViewFileMulti
{
protected:
	typedef struct _FILE_CELL
	{

		CMapViewFile*	lpMapView;

		QWORD	qwCellAddressBegin;
		QWORD	qwCellAddressEnd;
		QWORD	qwMaxCellSize;
		QWORD	qwCellSize;

		fs::path	szFilename;

	}FILE_CELL, *LPFILE_CELL;

private:
	typedef struct _CROSS_VIEW
	{
		LPBYTE	lpBufferTarget;
		LPBYTE	lpBufferTargetPtr;
		LPVOID	lpMapAddress;
		size_t	size;
		int		iCellID;
	}CROSS_VIEW, *LPCROSS_VIEW;

public:
	CMapViewFileMulti();
	virtual ~CMapViewFileMulti() noexcept;

	void	SetFilePointer(LONGLONG lDistanceToMove, DWORD dwMoveMethod = FILE_BEGIN);
	virtual	QWORD	GetFileSize() const;

	void	UnmapViewAll();
	void	UnMaping();
	void	clear();

	DWORD	Read(LPVOID buffer, DWORD dwBytesToRead);

	//��ȡ��ǰ�ļ��Ĵ�����
	const char*	GetFileDiskName() const;

	//����cell�����ʹ�С
	DWORD	GetCellCount() const;
	DWORD	GetCellSize() const;

protected:
	//��ǰ�ļ�ָ��λ��
	UNQWORD	m_uqwCurrentPos = 0;
	//��ǰ�Ѵ��ļ��ܴ�С
	UNQWORD m_uqwFullSize = 0;
	//��ǰ�����ļ�������С
	UNQWORD m_uqwMaxSize = 0;

	std::vector<FILE_CELL>	m_file_cell;
	std::vector<CROSS_VIEW>	m_cross_view;

	int	GetCellIDByPoint(QWORD qwPoint);

	LPBYTE	View(QWORD dwAddress, DWORD dwSize, BOOL isReadOnly);

private:
	BOOL	BuildCrossViewBuffer(LPBYTE lpCrossBuffer, LPBYTE &lpCrossBufferPtr, int cell_id, QWORD qwAddress, DWORD dwSize, BOOL isReadOnly);
	
};

class CMapViewFileMultiRead : public CMapViewFileMulti
{
public:
	CMapViewFileMultiRead();
	virtual ~CMapViewFileMultiRead() noexcept;

	BOOL	AddFile(const fs::path& lpszFilename);

	BOOL	Mapping();
	LPBYTE	View(QWORD dwAddress, DWORD dwSize);

private:

	BOOL	AddFile(CMapViewFileRead *lpRead, const fs::path& lpszFilename);

};

class CMapViewFileMultiWrite : public CMapViewFileMulti
{

public:
	CMapViewFileMultiWrite();
	virtual ~CMapViewFileMultiWrite() noexcept;

	BOOL	AddFile(const fs::path& lpszFilename, DWORD dwCreationDisposition, QWORD qwMaxSize, BOOL isNTFSSparseFile = FALSE);

	BOOL	Mapping(QWORD dwMaxSize);
	LPBYTE	View(QWORD dwAddress, DWORD dwSize);

	BOOL	SetEndOfFile();

	DWORD	Write(LPVOID buffer, DWORD dwBytesToWrite);

	BOOL	Write2(QWORD dwAddress, const void* buffer, DWORD dwBytesToWrite);

private:

	BOOL	AddFile(CMapViewFileWrite *lpWrite, QWORD qwMaxSize, const fs::path& lpszFilename);

	//qwCurrentPckFilesizeΪ�Ѿ����ڵ��ļ���С��qwToAddSpace����Ҫ����Ĵ�С������ֵΪ��qwCurrentPckFilesize + ���������������С��
	QWORD	GetExpanedPckFilesize(QWORD qwDiskFreeSpace, QWORD qwToAddSpace, QWORD qwCurrentPckFilesize);

	//ѹ���ؽ���ѹ��д��ռ䲻��ʱ��չ�ռ�
	BOOL	IsNeedExpandWritingFile(
		QWORD dwWritingAddressPointer,
		QWORD dwFileSizeToWrite);

	BOOL	ViewAndWrite2(QWORD dwAddress, const void *  buffer, DWORD dwSize);
};


