//////////////////////////////////////////////////////////////////////
// threadproc.cpp: WinPCK �����̲߳���
// ѹ������ѹ�����µȴ���ʱ���̣�����̵߳��� 
//
// �˳����� �����/stsm/liqf ��д
//
// �˴���Ԥ�ƽ��ῪԴ���κλ��ڴ˴�����޸ķ����뱣��ԭ������Ϣ
// 
// 2012.4.10
// 2012.10.10
//////////////////////////////////////////////////////////////////////

#pragma warning ( disable : 4244 )
#pragma warning ( disable : 4311 )
//#pragma warning ( disable : 4018 )
#pragma warning ( disable : 4005 )

#include "guipch.h"
#include "winmain.h"
#include "tRebuildOptDlg.h"
#include "tStripDlg.h"

fs::path GetPckFileNameBySource(fs::path src, BOOL isDirectory);

VOID TInstDlg::UpdatePckFile(VOID *pParam)
{

	TInstDlg	*pThis = (TInstDlg*)pParam;
	BOOL		bHasPckOpened = !pck_IsValidPck();
	std::wstring		szFilenameToSave;
	CStopWatch	timer;

	if (bHasPckOpened) {
		//��ʱû�д��ļ�����ʱ�Ĳ����൱���½��ĵ�
		pThis->m_currentNodeOnShow = NULL;

		if (1 == pThis->m_lpszFilePath.size()) {
			szFilenameToSave = GetPckFileNameBySource(pThis->m_lpszFilePath[0], FALSE).c_str();
		}

		try {
			//ѡ�񱣴���ļ���
			int nSelectFilter = ::SaveFile<wchar_t>(pThis->hWnd, szFilenameToSave, std::wstring(L"pck"), pThis->BuildSaveDlgFilterString());

			//�趨Ŀ��pck�İ汾
			if (WINPCK_OK != pck_setVersion(nSelectFilter))
				return;
		}
		catch (std::exception)
		{
			pck_close();
			return;
		}

	}
	else {
		szFilenameToSave.assign(pThis->m_Filename);
	}

	//��ʼ��ʱ
	timer.start();

	pThis->EnbaleButtons(FALSE);

	fs::path filepath(szFilenameToSave);

	pThis->SetStatusBarInfo(std::vformat(::GetLoadStrW(IDS_STRING_RENEWING), std::make_wformat_args(filepath.filename().wstring())).c_str());

	pThis->SetTimer(WM_TIMER_PROGRESS_100, TIMER_PROGRESS, NULL);

	pck_StringArrayReset();
	for (int i = 0; i < pThis->m_lpszFilePath.size(); i++) {
		pck_StringArrayAppend(pThis->m_lpszFilePath[i].c_str());
	}

	if (WINPCK_OK == pck_UpdatePckFileSubmit(szFilenameToSave.c_str(), pThis->m_currentNodeOnShow)) {

		//��ʱ����
		timer.stop();

		if (pck_isLastOptSuccess()) {

			pck_logN(::GetLoadStrW(IDS_STRING_RENEWOK), timer.getElapsedTime());
		}
		else {
			pThis->SetStatusBarInfo(::GetLoadStrW(IDS_STRING_PROCESS_ERROR));
		}


		//���ϱ���
		// ��ӡ����
		// pck����ԭ���ļ� %d ��\r\n
		// �¼����ļ� %d ���������ظ��ļ��� %d ��\r\n
		// ʹ��ԭ���ݵ�ַ %d ���������ݵ�ַ %d ��\r\n
		// ͨ���ɵ���pck�ļ��� %d ���ļ�\r\n

		if (0 != pck_getUpdateResult_PrepareToAddFileCount()) {

			auto sUpdateDetail = std::format(
				L"�˸��¹����������£�\r\n"
				L"PCK ����ԭ���ļ����� {}\r\n"
				L"�ƻ������ļ����� {}\r\n"
				L"ʵ�ʸ����ļ����� {}\r\n"
				L"�����ļ����� {}\r\n"
				L"δ�����ļ����� {}\r\n"
				L"���º� PCK �����ļ����� {}",
				pck_getUpdateResult_OldFileCount(),
				pck_getUpdateResult_PrepareToAddFileCount(),
				pck_getUpdateResult_ChangedFileCount(),
				pck_getUpdateResult_DuplicateFileCount(),
				pck_getUpdateResult_PrepareToAddFileCount() - pck_getUpdateResult_ChangedFileCount(),
				pck_getUpdateResult_FinalFileCount()
			);

			pThis->MessageBoxW(sUpdateDetail.c_str(), L"���±���", MB_OK | MB_ICONINFORMATION);

			pck_logI(sUpdateDetail.c_str());
		}


		if (bHasPckOpened) {

			pThis->OpenPckFile(szFilenameToSave.c_str(), TRUE);
		}
		else {
			pThis->OpenPckFile(TRUE);
		}

	}
	else {

		pThis->SetStatusBarInfo(GetLoadStr(IDS_STRING_PROCESS_ERROR));
		if (bHasPckOpened) {
			pck_close();
		}
	}

	pThis->EnbaleButtons(TRUE);

	pThis->KillTimer(WM_TIMER_PROGRESS_100);
	pThis->RefreshProgress();

	if ((!(pck_isLastOptSuccess())) && pThis->bGoingToExit) {
		pThis->bGoingToExit = FALSE;
		pThis->SendMessage(WM_CLOSE, 0, 0);
	}

	//��ԭDrop״̬
	pThis->m_lpszFilePath.clear();
	DragAcceptFiles(pThis->hWnd, TRUE);

	return;

}

VOID TInstDlg::RenamePckFile(VOID *pParam)
{

	TInstDlg	*pThis = (TInstDlg*)pParam;
	CStopWatch	timer;
	//��ʼ��ʱ
	timer.start();

	pThis->EnbaleButtons(FALSE);

	pThis->SetStatusBarInfo(std::vformat(::GetLoadStrW(IDS_STRING_RENEWING), std::make_wformat_args(pThis->m_Filename.filename().wstring())).c_str());

	pThis->SetTimer(WM_TIMER_PROGRESS_100, TIMER_PROGRESS, NULL);

	if (WINPCK_OK == pck_RenameSubmit()) {

		//��ʱ����
		timer.stop();

		pThis->SetStatusBarInfo(std::vformat(::GetLoadStrW(IDS_STRING_RENEWOK), std::make_wformat_args(timer.getElapsedTime())).c_str());

		pThis->OpenPckFile(TRUE);

	}
	else {
		pThis->SetStatusBarInfo(::GetLoadStrW(IDS_STRING_PROCESS_ERROR));
		pck_close();
	}

	pThis->EnbaleButtons(TRUE);

	pThis->KillTimer(WM_TIMER_PROGRESS_100);
	pThis->RefreshProgress();

	if (pThis->bGoingToExit) {
		pThis->bGoingToExit = FALSE;
		pThis->SendMessage(WM_CLOSE, 0, 0);
	}

	return;
}

VOID TInstDlg::RebuildPckFile(VOID	*pParam)
{
	try {

		TInstDlg* pThis = (TInstDlg*)pParam;

		BOOL		bNeedCreatePck = !pck_IsValidPck();

		TCHAR		szScriptFile[MAX_PATH];
		CStopWatch	timer;

		if (bNeedCreatePck) {
			if (!pThis->NewPckFile()) {
				return;
			}
		}

		fs::path filepath(pThis->m_Filename);
		filepath.replace_filename(L"Rebuild_" + filepath.filename().wstring());

		std::wstring szFilenameToSave(filepath.wstring());

		//����ѡ��Ի���
		//���öԻ���
		BOOL  bNeedRecompress;
		TRebuildOptDlg	dlg(szScriptFile, &bNeedRecompress, pThis);
		if (IDCANCEL == dlg.Exec())
			return;

		//ѡ�񱣴���ļ���
		int nSelectFilter = ::SaveFile<wchar_t>(pThis->hWnd, szFilenameToSave, std::wstring(L"pck"), pThis->BuildSaveDlgFilterString(), pck_getVersion());

		if (0 > nSelectFilter) {
			return;
		}

		if (WINPCK_OK != pck_setVersion(nSelectFilter))
			return;

		//��ʼ��ʱ
		timer.start();

		pThis->EnbaleButtons(FALSE);

		filepath = fs::path(szFilenameToSave);

		pThis->SetStatusBarInfo(std::vformat(::GetLoadStrW(IDS_STRING_REBUILDING), std::make_wformat_args(filepath.filename().wstring())).c_str());

		pThis->SetTimer(WM_TIMER_PROGRESS_100, 100, NULL);

		if (WINPCK_OK == pck_RebuildPckFileWithScript(szScriptFile, szFilenameToSave.c_str(), bNeedRecompress)) {

			//��ʱ����
			timer.stop();

			if (pck_isLastOptSuccess()) {

				pck_logN(GetLoadStr(IDS_STRING_REBUILDOK), timer.getElapsedTime());
			}
			else {
				pThis->SetStatusBarInfo(GetLoadStr(IDS_STRING_PROCESS_ERROR));
			}

		}
		else {
			pThis->SetStatusBarInfo(GetLoadStr(IDS_STRING_PROCESS_ERROR));
		}

		pThis->EnbaleButtons(TRUE);

		if (bNeedCreatePck) {
			ListView_DeleteAllItems(pThis->GetDlgItem(IDC_LIST));
			pck_close();
		}

		pThis->KillTimer(WM_TIMER_PROGRESS_100);
		pThis->RefreshProgress();

		if ((!(pck_isLastOptSuccess())) && pThis->bGoingToExit) {
			pThis->bGoingToExit = FALSE;
			pThis->SendMessage(WM_CLOSE, 0, 0);
		}
	}
	catch (std::exception ex)
	{
		pck_logIA(ex.what());
	}

	return;
}

VOID TInstDlg::StripPckFile(VOID *pParam)
{

	TInstDlg	*pThis = (TInstDlg*)pParam;
	BOOL		bNeedCreatePck = !pck_IsValidPck();
	CStopWatch	timer;

	if (bNeedCreatePck) {
		if (!pThis->NewPckFile()) {
			return;
		}
	}

	fs::path filepath(pThis->m_Filename);
	filepath.replace_filename(L"Striped_" + filepath.filename().wstring());

	std::wstring szFilenameToSave(filepath.wstring());

	//����ѡ��Ի���
	//���öԻ���
	int stripFlag;
	TStripDlg	dlg(&stripFlag, pThis);
	if (IDCANCEL == dlg.Exec())
		return;

	try {
		//ѡ�񱣴���ļ���
		int nSelectFilter = ::SaveFile<wchar_t>(pThis->hWnd, szFilenameToSave, std::wstring(L"pck"), pThis->BuildSaveDlgFilterString(), pck_getVersion());

		if (WINPCK_OK != pck_setVersion(nSelectFilter))
			return;
	}
	catch (std::exception)
	{
		return;
	}

	//��ʼ��ʱ
	timer.start();

	pThis->EnbaleButtons(FALSE);

	filepath = fs::path(szFilenameToSave);
	pThis->SetStatusBarInfo(std::vformat(::GetLoadStrW(IDS_STRING_REBUILDING), std::make_wformat_args(filepath.filename().wstring())).c_str());

	pThis->SetTimer(WM_TIMER_PROGRESS_100, 100, NULL);

	if (WINPCK_OK == pck_StripPck(szFilenameToSave.c_str(), stripFlag)) {

		//��ʱ����
		timer.stop();

		if (pck_isLastOptSuccess()) {

			pck_logN(GetLoadStr(IDS_STRING_REBUILDOK), timer.getElapsedTime());
		}
		else {
			pThis->SetStatusBarInfo(GetLoadStr(IDS_STRING_PROCESS_ERROR));
		}

	}
	else {
		pThis->SetStatusBarInfo(GetLoadStr(IDS_STRING_PROCESS_ERROR));
	}

	pThis->EnbaleButtons(TRUE);

	if (bNeedCreatePck) {
		ListView_DeleteAllItems(pThis->GetDlgItem(IDC_LIST));
		pck_close();
	}

	pThis->KillTimer(WM_TIMER_PROGRESS_100);
	pThis->RefreshProgress();

	if ((!(pck_isLastOptSuccess())) && pThis->bGoingToExit) {
		pThis->bGoingToExit = FALSE;
		pThis->SendMessage(WM_CLOSE, 0, 0);
	}
	return;
}

VOID TInstDlg::CreateNewPckFile(VOID	*pParam)
{
	TInstDlg	*pThis = (TInstDlg*)pParam;

	BOOL		isNotOpenedPck = !pck_IsValidPck();

	CStopWatch	timer;

	try {
		//ѡ��Ŀ¼
		CDlgOpenDirectory cDlgOpenDirectory;
		pThis->m_CurrentPath = cDlgOpenDirectory.Show(pThis->hWnd);
	}
	catch (std::exception) {
		return;
	}

	pThis->m_lpszFilePath.push_back(pThis->m_CurrentPath);

	auto szFilenameToSave = GetPckFileNameBySource(pThis->m_CurrentPath, TRUE).wstring();

	try {
		//ѡ�񱣴���ļ���
		int nSelectFilter = ::SaveFile<wchar_t>(pThis->hWnd, szFilenameToSave, std::wstring(L"pck"), pThis->BuildSaveDlgFilterString());

		//�趨Ŀ��pck�İ汾
		if (WINPCK_OK != pck_setVersion(nSelectFilter))
			return;
	}
	catch (std::exception) {
		return;
	}

	//��ʼ��ʱ
	timer.start();

	pThis->EnbaleButtons(FALSE);

	fs::path filenameToSave(szFilenameToSave);

	pThis->SetStatusBarInfo(std::vformat(::GetLoadStrW(IDS_STRING_COMPING), std::make_wformat_args( filenameToSave.filename().wstring())).c_str());

	pThis->SetTimer(WM_TIMER_PROGRESS_100, TIMER_PROGRESS, NULL);

	pck_StringArrayReset();
	for (int i = 0; i < pThis->m_lpszFilePath.size(); i++) {
		pck_StringArrayAppend(pThis->m_lpszFilePath[i].c_str());
	}

	if (WINPCK_OK == pck_UpdatePckFileSubmit(szFilenameToSave.c_str(), pThis->m_currentNodeOnShow)) {

		//��ʱ����
		timer.stop();

		if (pck_isLastOptSuccess()) {
			pck_logN(GetLoadStr(IDS_STRING_REBUILDOK), timer.getElapsedTime());
		}
		else {
			pThis->SetStatusBarInfo(GetLoadStrW(IDS_STRING_PROCESS_ERROR));
		}

	}
	else {
		pThis->SetStatusBarInfo(GetLoadStrW(IDS_STRING_PROCESS_ERROR));
	}

	pThis->EnbaleButtons(TRUE);

	if (isNotOpenedPck)
		pck_close();

	pThis->KillTimer(WM_TIMER_PROGRESS_100);
	pThis->RefreshProgress();

	if ((!(pck_isLastOptSuccess())) && pThis->bGoingToExit) {

		pThis->bGoingToExit = FALSE;
		pThis->SendMessage(WM_CLOSE, 0, 0);
	}
	return;
}

VOID TInstDlg::ToExtractAllFiles(VOID	*pParam)
{
	TInstDlg	*pThis = (TInstDlg*)pParam;

	CStopWatch	timer;

	//��ʼ��ʱ
	timer.start();

	pThis->EnbaleButtons(FALSE);

	fs::path filepath(pThis->m_Filename);
	pThis->SetStatusBarInfo(std::vformat(::GetLoadStrW(IDS_STRING_EXPING), std::make_wformat_args(filepath.filename().wstring())).c_str());

	pThis->SetTimer(WM_TIMER_PROGRESS_100, TIMER_PROGRESS, NULL);

	if (WINPCK_OK == pck_ExtractAllFiles(pThis->m_CurrentPath.c_str())) {
		//��ʱ����
		timer.stop();
		pThis->SetStatusBarInfo(std::vformat(::GetLoadStrW(IDS_STRING_EXPOK), std::make_wformat_args(timer.getElapsedTime())).c_str());

	}
	else {

		pThis->SetStatusBarInfo(GetLoadStr(IDS_STRING_PROCESS_ERROR));
	}

	pThis->EnbaleButtons(TRUE);

	pThis->KillTimer(WM_TIMER_PROGRESS_100);
	pThis->RefreshProgress();

	if ((!(pck_isLastOptSuccess())) && pThis->bGoingToExit) {
		pThis->bGoingToExit = FALSE;
		pThis->SendMessage(WM_CLOSE, 0, 0);
	}

	return;
}

VOID TInstDlg::ToExtractSelectedFiles(VOID	*pParam)
{
	TInstDlg	*pThis = (TInstDlg*)pParam;

	HWND	hList = pThis->GetDlgItem(IDC_LIST);

	CStopWatch	timer;

	const PCK_UNIFIED_FILE_ENTRY **lpFileEntryArray, **lpFileEntryArrayPtr;

	UINT uiSelectCount = ListView_GetSelectedCount(hList);

	if (0 != uiSelectCount) {
		if (NULL != (lpFileEntryArray = (const PCK_UNIFIED_FILE_ENTRY **)malloc(sizeof(PCK_UNIFIED_FILE_ENTRY *) * uiSelectCount))) {

			//ȡlpNodeToShow
			int	nCurrentItemCount = ListView_GetItemCount(hList);

			LVITEM item;
			item.mask = LVIF_PARAM | LVIF_STATE;
			item.iSubItem = 0;
			item.stateMask = LVIS_SELECTED;		// get all state flags

			lpFileEntryArrayPtr = lpFileEntryArray;

			uiSelectCount = 0;

			//��1��ʼ������..Ŀ¼
			for (item.iItem = 1; item.iItem < nCurrentItemCount; item.iItem++) {
				ListView_GetItem(hList, &item);

				if (item.state & LVIS_SELECTED) {
					*lpFileEntryArrayPtr = (LPPCK_UNIFIED_FILE_ENTRY)item.lParam;
					lpFileEntryArrayPtr++;

					uiSelectCount++;
				}
			}

			if (0 == uiSelectCount)return;

			//��ʼ��ʱ
			timer.start();

			pThis->EnbaleButtons(FALSE);

			fs::path filepath(pThis->m_Filename);
			pThis->SetStatusBarInfo(std::vformat(::GetLoadStrW(IDS_STRING_EXPING), std::make_wformat_args(filepath.filename().wstring())).c_str());

			pThis->SetTimer(WM_TIMER_PROGRESS_100, TIMER_PROGRESS, NULL);

			lpFileEntryArrayPtr = lpFileEntryArray;

			if (WINPCK_OK == pck_ExtractFilesByEntrys(lpFileEntryArray, uiSelectCount, pThis->m_CurrentPath.c_str())) {
				//��ʱ����
				timer.stop();
				pThis->SetStatusBarInfo(std::vformat(::GetLoadStrW(IDS_STRING_EXPOK), std::make_wformat_args(timer.getElapsedTime())).c_str());
			}
			else {
				pThis->SetStatusBarInfo(GetLoadStr(IDS_STRING_PROCESS_ERROR));
			}

			free(lpFileEntryArray);

			pThis->EnbaleButtons(TRUE);

			pThis->KillTimer(WM_TIMER_PROGRESS_100);
			pThis->RefreshProgress();

			if ((!(pck_isLastOptSuccess())) && pThis->bGoingToExit) {
				pThis->bGoingToExit = FALSE;
				pThis->SendMessage(WM_CLOSE, 0, 0);
			}
		}
	}
	return;
}


VOID TInstDlg::DeleteFileFromPckFile(VOID	*pParam)
{
	TInstDlg	*pThis = (TInstDlg*)pParam;

	HWND	hList = pThis->GetDlgItem(IDC_LIST);

	UINT uiSelectCount = ListView_GetSelectedCount(hList);

	if (0 != uiSelectCount) {

		int	nCurrentItemCount = ListView_GetItemCount(hList);

		LVITEMW item {
			.mask = LVIF_PARAM | LVIF_STATE,
			.iSubItem = 0,
			.stateMask = LVIS_SELECTED,		// get all state flags
		};

		uiSelectCount = 0;

		for (item.iItem = 1; item.iItem < nCurrentItemCount; item.iItem++) {
			ListView_GetItem(hList, &item);

			if (item.state & LVIS_SELECTED) {
				//WINPCK_OK
				pck_DeleteEntry((LPPCK_UNIFIED_FILE_ENTRY)item.lParam);
				uiSelectCount++;
			}
		}

		if (0 == uiSelectCount)return;

		CStopWatch	timer;
		//��ʼ��ʱ
		timer.start();

		pThis->EnbaleButtons(FALSE);

		pThis->SetStatusBarInfo(std::vformat(::GetLoadStrW(IDS_STRING_RENEWING), std::make_wformat_args(pThis->m_Filename.filename().wstring())).c_str());

		pThis->SetTimer(WM_TIMER_PROGRESS_100, TIMER_PROGRESS, NULL);

		if (WINPCK_OK == pck_RenameSubmit()) {
			//��ʱ����
			timer.stop();
			pThis->SetStatusBarInfo(std::vformat(::GetLoadStrW(IDS_STRING_RENEWOK), std::make_wformat_args(timer.getElapsedTime())).c_str());
			pThis->OpenPckFile(TRUE);

		}
		else {

			pThis->SetStatusBarInfo(::GetLoadStrW(IDS_STRING_PROCESS_ERROR));
		}

		pThis->EnbaleButtons(TRUE);

		pThis->KillTimer(WM_TIMER_PROGRESS_100);
		pThis->RefreshProgress();

		if (pThis->bGoingToExit) {
			pThis->bGoingToExit = FALSE;
			pThis->SendMessage(WM_CLOSE, 0, 0);
		}

	}
	return;
}

//�������Դ�ļ����Ƴ�Ԥ�����pck�ļ���
fs::path GetPckFileNameBySource(fs::path src, BOOL isDirectory)
{
	constexpr wchar_t file_end_with[] = L".pck.files";
	constexpr auto filepath_min_len = sizeof("c:\\") - 1 + sizeof(file_end_with) / sizeof(wchar_t) - 1;

	fs::path dst = src;

	if (isDirectory) {

		if (dst.wstring().ends_with(file_end_with) && dst.wstring().size() > filepath_min_len)
		{
			dst.replace_extension("");
		}

	}
	else {
		dst.replace_extension(dst.extension().string() + ".pck");
	}
	return dst;
}