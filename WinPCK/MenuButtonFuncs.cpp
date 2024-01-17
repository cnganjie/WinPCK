//////////////////////////////////////////////////////////////////////
// MenuButtonFuncs.cpp: WinPCK �����̲߳���
// ���水ť����
//
// �˳����� �����/stsm/liqf ��д
//
// �˴��뿪Դ���κλ��ڴ˴�����޸ķ����뱣��ԭ������Ϣ
// 
// 2018.9.4
//////////////////////////////////////////////////////////////////////

#include "guipch.h"
#include "winmain.h"
#include "tInfoDlg.h"
#include "tSearchDlg.h"
#include "tCompressOptDlg.h"

void TInstDlg::ListViewEnter()
{
	if (!this->m_isListviewRenaming)
		this->DbClickListView(this->m_iListHotItem);
	else
		::keybd_event(VK_TAB, 0, 0, 0);
}

void TInstDlg::MenuClose()
{
	if (pck_isThreadWorking())
		pck_forceBreakThreadWorking();

	ListView_DeleteAllItems(this->GetDlgItem(IDC_LIST));
	pck_close();
}

void TInstDlg::MenuInfo()
{
	if (pck_IsValidPck()) {
		TInfoDlg	dlg(this);
		dlg.Exec();
	}
}

void TInstDlg::MenuSearch()
{
	if (pck_IsValidPck()) {
		TSearchDlg	dlg(this->m_szStrToSearch, this);
		if (dlg.Exec() == TRUE) {
			this->SearchPckFiles();
		}
	}
}

void TInstDlg::MenuNew(WORD wID)
{
		
	if (pck_isThreadWorking())
		return;

	_beginthread(CreateNewPckFile, 0, this);
}

void TInstDlg::MenuAdd(WORD wID)
{
	if (pck_isThreadWorking())
		return;

	this->AddFiles();
}

void TInstDlg::MenuRebuild(WORD wID)
{
	if (pck_isThreadWorking())
		return;

	//pck_setMTMemoryUsed(m_PckHandle, 0);
	_beginthread(RebuildPckFile, 0, this);
}

void TInstDlg::MenuCompressOpt()
{
	TCompressOptDlg	dlg(this);
	dlg.Exec();
}

void TInstDlg::MenuRename()
{
	if(pck_isThreadWorking())
		return;

	HWND	hList = this->GetDlgItem(IDC_LIST);
	::SetWindowLong(hList, GWL_STYLE, ::GetWindowLong(hList, GWL_STYLE) | LVS_EDITLABELS);
	ListView_EditLabel(hList, this->m_iListHotItem);
}

void TInstDlg::MenuDelete()
{
	if (pck_isThreadWorking())
		return;
	if (IDNO == this->MessageBoxW(::GetLoadStrW(IDS_STRING_ISDELETE), ::GetLoadStrW(IDS_STRING_ISDELETETITLE), MB_YESNO | MB_ICONEXCLAMATION | MB_DEFBUTTON2))
		return;

	//pck_setMTMemoryUsed(m_PckHandle, 0);
	_beginthread(DeleteFileFromPckFile, 0, this);
}

void TInstDlg::MenuSelectAll()
{
	LVITEM item = { 0 };

	item.mask = LVIF_STATE;
	item.stateMask = item.state = LVIS_SELECTED;

	HWND hList = this->GetDlgItem(IDC_LIST);

	int	nItemCount = ListView_GetItemCount(hList);

	for (item.iItem = 1; item.iItem < nItemCount; item.iItem++) {
		ListView_SetItem(hList, &item);
	}
}

void TInstDlg::MenuSelectReverse()
{
	LVITEMW item = { 0 };

	item.mask = LVIF_STATE;
	item.stateMask = LVIS_SELECTED;

	HWND hList = this->GetDlgItem(IDC_LIST);

	int	nItemCount = ListView_GetItemCount(hList);

	for (item.iItem = 1; item.iItem < nItemCount; item.iItem++) {
		ListView_GetItem(hList, &item);
		item.state = LVIS_SELECTED == item.state ? 0 : LVIS_SELECTED;
		ListView_SetItem(hList, &item);
	}
}

void TInstDlg::MenuAttribute()
{
	if (pck_isThreadWorking())
		return;

	this->ViewFileAttribute();
}

void TInstDlg::MenuView()
{
	if (pck_isThreadWorking())
		return;

	HWND	hWndList = this->GetDlgItem(IDC_LIST);

	LVITEM	item = { 0 };

	item.mask = LVIF_PARAM;
	item.iItem = m_iListHotItem;
	ListView_GetItem(hWndList, &item);

	int entry_type = ((LPPCK_UNIFIED_FILE_ENTRY)item.lParam)->entryType;

	if (PCK_ENTRY_TYPE_FOLDER == entry_type)
		return;

	this->ViewFile((LPPCK_UNIFIED_FILE_ENTRY)item.lParam);
}

void TInstDlg::MenuAbout()
{
	MessageBoxW(THIS_MAIN_CAPTION
		"\r\n"
		THIS_DESC
		"\r\n\r\n"
		THIS_AUTHOR
		"\r\n\r\n"
		THIS_UESDLIB,
		L"���� "
		THIS_NAME,
		MB_OK | MB_ICONASTERISK);
}

//��Ϸ����
void TInstDlg::MenuStrip()
{
	if (pck_isThreadWorking())
		return;

	_beginthread(StripPckFile, 0, this);
}

//ȡ������
void TInstDlg::MenuCancelPckOper()
{
	if (pck_isThreadWorking()) {
		pck_forceBreakThreadWorking();
		//EnableButton(wID, FALSE);
	}
	else {
		this->EnbaleButtons(TRUE);
	}
}