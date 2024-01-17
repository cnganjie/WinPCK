//////////////////////////////////////////////////////////////////////
// tAttrDlg.cpp: WinPCK �����̲߳���
// �Ի������
//
// �˳����� �����/stsm/liqf ��д
//
// �˴���Ԥ�ƽ��ῪԴ���κλ��ڴ˴�����޸ķ����뱣��ԭ������Ϣ
// 
// 2017.12.26
//////////////////////////////////////////////////////////////////////

#include "guipch.h"
#include "tAttrDlg.h"


TAttrDlg::TAttrDlg(const PCK_UNIFIED_FILE_ENTRY *_lpPckInfo, wchar_t *_lpszPath, TWin *_win) :
	TDlg(IDD_DIALOG_ATTR, _win),
	lpPckInfo(_lpPckInfo),
	lpszPath(_lpszPath)
{
}

BOOL TAttrDlg::EvCreate(LPARAM lParam)
{
	constexpr wchar_t	lpszFormat[] = L"{} ({} �ֽ�)";
	constexpr wchar_t	lpszFileFormat[] = L"{} ({} �ֽ�)";
	std::wstring sFilename;

	if (PCK_ENTRY_TYPE_INDEX == this->lpPckInfo->entryType) {

		this->SetDlgItemTextW(IDC_EDIT_ATTR_TYPE, L"�ļ�");

		fs::path searched_filename(this->lpPckInfo->szName);
		sFilename = searched_filename.filename().wstring();
		this->SetDlgItemTextW(IDC_EDIT_ATTR_PATH, searched_filename.parent_path().wstring().c_str());
		this->SetDlgItemTextW(IDC_EDIT_ATTR_NAME, searched_filename.filename().wstring().c_str());
	}
	else {
		sFilename = this->lpPckInfo->szName;
		this->SetDlgItemTextW(IDC_EDIT_ATTR_NAME, this->lpPckInfo->szName);
		this->SetDlgItemTextW(IDC_EDIT_ATTR_PATH, this->lpszPath);
	}

	uint64_t qdwDirClearTextSize = pck_getFileSizeInEntry(this->lpPckInfo);
	uint64_t qdwDirCipherTextSize = pck_getCompressedSizeInEntry(this->lpPckInfo);

	//ѹ����С
	this->SetDlgItemTextW(IDC_EDIT_ATTR_CIPHER, std::vformat(lpszFileFormat, std::make_wformat_args(::wbyte_format(qdwDirCipherTextSize), qdwDirCipherTextSize)).c_str());
	//ʵ�ʴ�С
	this->SetDlgItemTextW(IDC_EDIT_ATTR_SIZE, std::vformat(lpszFileFormat, std::make_wformat_args(::wbyte_format(qdwDirClearTextSize), qdwDirClearTextSize)).c_str());

	//ѹ����
	if (0 == qdwDirClearTextSize)
		this->SetDlgItemTextA(IDC_EDIT_ATTR_CMPR, "-");
	else {
		this->SetDlgItemTextW(IDC_EDIT_ATTR_CMPR, std::format(L"{:.1f}%", qdwDirCipherTextSize / (double)qdwDirClearTextSize * 100).c_str());
	}

	if (PCK_ENTRY_TYPE_FOLDER != (PCK_ENTRY_TYPE_FOLDER & this->lpPckInfo->entryType))//�ļ�
	{
		this->SetDlgItemTextW(IDC_EDIT_ATTR_TYPE, L"�ļ�");

		//����
		this->SetDlgItemTextA(IDC_EDIT_ATTR_FILECOUNT, "-");

		//ƫ�Ƶ�ַ
		this->SetDlgItemTextW(IDC_EDIT_ATTR_ADDR, std::format(L"{} (0x{:016X})", pck_getFileOffset(this->lpPckInfo), pck_getFileOffset(this->lpPckInfo)).c_str());
	}
	else {

		this->SetDlgItemTextW(IDC_EDIT_ATTR_TYPE, L"�ļ���");

		//����
		this->SetDlgItemTextW(IDC_EDIT_ATTR_FILECOUNT, std::format(L"{} ���ļ���{} ���ļ���", pck_getFilesCountInEntry(this->lpPckInfo), pck_getFoldersCountInEntry(this->lpPckInfo)).c_str());

		//ƫ�Ƶ�ַ
		this->SetDlgItemTextA(IDC_EDIT_ATTR_ADDR, "-");

	}

	//�ļ�����Ϣ
	const PCK_UNIFIED_FILE_ENTRY* lpRootNode = pck_getRootNode();

	qdwDirClearTextSize = pck_getFileSizeInEntry(lpRootNode);
	qdwDirCipherTextSize = pck_getCompressedSizeInEntry(lpRootNode);

	//�ļ��ܴ�С
	this->SetDlgItemTextW(IDC_EDIT_ATTR_ALLSIZE, std::vformat(lpszFormat, std::make_wformat_args(::wbyte_format(qdwDirClearTextSize), qdwDirClearTextSize)).c_str());

	//ѹ������С
	this->SetDlgItemTextW(IDC_EDIT_ATTR_PCKSIZE, std::vformat(lpszFormat, std::make_wformat_args(::wbyte_format(qdwDirCipherTextSize), qdwDirCipherTextSize)).c_str());

	//ѹ����
	this->SetDlgItemTextW(IDC_EDIT_ATTR_PCKCMPR, std::format(L"{:.1f}%", qdwDirCipherTextSize / (float)qdwDirClearTextSize * 100.0).c_str());

	//����������
	auto const qwPckFileSize = pck_file_redundancy_data_size();
	this->SetDlgItemTextW(IDC_EDIT_ATTR_DIRT, std::vformat(lpszFormat, std::make_wformat_args(::wbyte_format(qwPckFileSize), qwPckFileSize)).c_str());


	//��������
	this->SetWindowTextW(std::format(L"{} ����", sFilename).c_str());
	this->Show();

	return	FALSE;
}

BOOL TAttrDlg::EvCommand(WORD wNotifyCode, WORD wID, LPARAM hwndCtl)
{
	switch(wID) {
	case IDOK:
	case IDCANCEL:
		EndDialog(wID);
		return	TRUE;
	}
	return	FALSE;
}
