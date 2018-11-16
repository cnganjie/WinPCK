//////////////////////////////////////////////////////////////////////
// threadproc.cpp: WinPCK 界面线程部分
// 压缩、解压、更新等大用时过程，需多线程调用 
//
// 此程序由 李秋枫/stsm/liqf 编写
//
// 此代码预计将会开源，任何基于此代码的修改发布请保留原作者信息
// 
// 2012.4.10
// 2012.10.10
//////////////////////////////////////////////////////////////////////

#pragma warning ( disable : 4244 )
#pragma warning ( disable : 4311 )
//#pragma warning ( disable : 4018 )
#pragma warning ( disable : 4005 )

#include "tlib.h"
#include "resource.h"
#include "globals.h"
#include "winmain.h"
#include <process.h>
#include <tchar.h>
#include "OpenSaveDlg.h"
#include "StopWatch.h"

VOID GetPckFileNameBySource(LPTSTR dst, LPCTSTR src, BOOL isDirectory);

VOID TInstDlg::UpdatePckFile(VOID *pParam)
{

	TInstDlg	*pThis = (TInstDlg*)pParam;
	BOOL		bHasPckOpened = !pck_IsValidPck(pThis->m_PckHandle);
	TCHAR		szFilenameToSave[MAX_PATH];
	TCHAR		szPrintf[320];
	CStopWatch	timer;;

	*szFilenameToSave = 0;

	if (bHasPckOpened) {
		//此时没有打开文件，这时的操作相当于新建文档
		pThis->m_currentNodeOnShow = NULL;

		if (1 == pThis->m_lpszFilePath.size()) {
			GetPckFileNameBySource(szFilenameToSave, pThis->m_lpszFilePath[0].c_str(), FALSE);
		}

		//选择保存的文件名
		int nSelectFilter = SaveFile(pThis->hWnd, szFilenameToSave, TEXT("pck"), pThis->BuildSaveDlgFilterString());
		if (0 > nSelectFilter) {
			pck_close(pThis->m_PckHandle);
			return;
		}

		//设定目标pck的版本
		pck_setVersion(pThis->m_PckHandle, nSelectFilter);

		_stprintf_s(szPrintf, GetLoadStr(IDS_STRING_RENEWING), _tcsrchr(szFilenameToSave, TEXT('\\')) + 1);
	}
	else {
		_tcscpy_s(szFilenameToSave, pThis->m_Filename);
		_stprintf_s(szPrintf, GetLoadStr(IDS_STRING_RENEWING), _tcsrchr(pThis->m_Filename, TEXT('\\')) + 1);
	}

	//开始计时
	timer.start();

	pThis->EnbaleButtons(ID_MENU_ADD, FALSE);


	pThis->SetStatusBarText(4, szPrintf);

	//pck_setThreadWorking(pThis->m_PckHandle);

	pThis->SetTimer(WM_TIMER_PROGRESS_100, TIMER_PROGRESS, NULL);

	pck_StringArrayReset(pThis->m_PckHandle);
	for (int i = 0; i < pThis->m_lpszFilePath.size(); i++) {
		pck_StringArrayAppend(pThis->m_PckHandle, pThis->m_lpszFilePath[i].c_str());
	}

	if (pck_UpdatePckFileSubmit(pThis->m_PckHandle, szFilenameToSave, pThis->m_currentNodeOnShow)) {

		//计时结束
		timer.stop();

		if (pck_isLastOptSuccess(pThis->m_PckHandle)) {

			//pThis->m_PckLog.PrintLogN(GetLoadStr(IDS_STRING_RENEWOK), timer.getElapsedTime());
			log_Print(LOG_IMAGE_NOTICE, GetLoadStr(IDS_STRING_RENEWOK), timer.getElapsedTime());
			//pThis->SetStatusBarText(4, szPrintf);
		}
		else {

			pThis->SetStatusBarText(4, GetLoadStr(IDS_STRING_PROCESS_ERROR));
		}

		if (bHasPckOpened) {

			pThis->OpenPckFile(szFilenameToSave, TRUE);
		}
		else {
			pThis->OpenPckFile(pThis->m_Filename, TRUE);
		}

	}
	else {

		pThis->SetStatusBarText(4, GetLoadStr(IDS_STRING_PROCESS_ERROR));

		if (bHasPckOpened) {
			pck_close(pThis->m_PckHandle);
		}
	}

	pThis->EnbaleButtons(ID_MENU_ADD, TRUE);

	pThis->KillTimer(WM_TIMER_PROGRESS_100);
	pThis->RefreshProgress();

	if ((!(pck_isLastOptSuccess(pThis->m_PckHandle))) && pThis->bGoingToExit) {
		pThis->bGoingToExit = FALSE;
		pThis->SendMessage(WM_CLOSE, 0, 0);
	}

	//pck_breakThreadWorking(pThis->m_PckHandle);
	//*lpbThreadRunning = FALSE;

	//还原Drop状态
	pThis->m_lpszFilePath.clear();
	DragAcceptFiles(pThis->hWnd, TRUE);

	//整合报告
	// 打印报告
	// pck包中原有文件 %d 个\r\n
	// 新加入文件 %d 个，其中重复文件名 %d 个\r\n
	// 使用原数据地址 %d 个，新数据地址 %d 个\r\n
	// 通过成的新pck文件共 %d 个文件\r\n

	//const PCK_VARIETY_PARAMS* lpVarParams = pck_getUpdateResult(pThis->m_PckHandle);
	if (0 != pck_getUpdateResult_PrepareToAddFileCount(pThis->m_PckHandle)) {

		_stprintf_s(szPrintf,
			TEXT("此更新过程数据如下：\r\n")
			TEXT("PCK 包中原有文件数： %d\r\n")
			TEXT("计划更新文件数： %d\r\n")
			TEXT("实际更新文件数： %d\r\n")
			TEXT("重名文件数： %d\r\n")
			TEXT("未更新文件数： %d\r\n")
			TEXT("更新后 PCK 包中文件数： %d"),
			pck_getUpdateResult_OldFileCount(pThis->m_PckHandle),
			pck_getUpdateResult_PrepareToAddFileCount(pThis->m_PckHandle),
			pck_getUpdateResult_ChangedFileCount(pThis->m_PckHandle),
			pck_getUpdateResult_DuplicateFileCount(pThis->m_PckHandle),
			pck_getUpdateResult_PrepareToAddFileCount(pThis->m_PckHandle) - pck_getUpdateResult_ChangedFileCount(pThis->m_PckHandle),
			pck_getUpdateResult_FinalFileCount(pThis->m_PckHandle));

		pThis->MessageBox(szPrintf, TEXT("更新报告"), MB_OK | MB_ICONINFORMATION);

		//pThis->m_PckLog.PrintLogI(szPrintf);
		log_Print(LOG_IMAGE_INFO, szPrintf);
	}

	return;

}

VOID TInstDlg::RenamePckFile(VOID *pParam)
{

	TInstDlg	*pThis = (TInstDlg*)pParam;
	TCHAR		szPrintf[64];
	CStopWatch	timer;

	_stprintf_s(szPrintf, GetLoadStr(IDS_STRING_RENEWING), _tcsrchr(pThis->m_Filename, TEXT('\\')) + 1);

	//开始计时
	timer.start();

	pThis->EnbaleButtons(ID_MENU_RENAME, FALSE);

	pThis->SetStatusBarText(4, szPrintf);

	//pck_setThreadWorking(pThis->m_PckHandle);

	pThis->SetTimer(WM_TIMER_PROGRESS_100, TIMER_PROGRESS, NULL);

	if (pck_RenameSubmit(pThis->m_PckHandle)) {

		//计时结束
		timer.stop();
		_stprintf_s(szPrintf, GetLoadStr(IDS_STRING_RENEWOK), timer.getElapsedTime());

		pThis->SetStatusBarText(4, szPrintf);

		pThis->OpenPckFile(pThis->m_Filename, TRUE);

	}
	else {

		pThis->SetStatusBarText(4, GetLoadStr(IDS_STRING_PROCESS_ERROR));
		pck_close(pThis->m_PckHandle);
	}

	pThis->EnbaleButtons(ID_MENU_RENAME, TRUE);

	pThis->KillTimer(WM_TIMER_PROGRESS_100);
	pThis->RefreshProgress();

	if (pThis->bGoingToExit) {
		pThis->bGoingToExit = FALSE;
		pThis->SendMessage(WM_CLOSE, 0, 0);
	}

	//pck_breakThreadWorking(pThis->m_PckHandle);

	return;

}

VOID TInstDlg::RebuildPckFile(VOID	*pParam)
{

	TInstDlg	*pThis = (TInstDlg*)pParam;

	BOOL		bDeleteClass = !pck_IsValidPck(pThis->m_PckHandle);

	TCHAR		szFilenameToSave[MAX_PATH];
	TCHAR		szPrintf[288];

	CStopWatch	timer;

	if (bDeleteClass) {
		if (!pThis->OpenPckFile()) {
			return;
		}
	}

	_tcscpy_s(szFilenameToSave, pThis->m_Filename);

	TCHAR		*lpszFileTitle = _tcsrchr(szFilenameToSave, TEXT('\\')) + 1;
	_tcscpy(lpszFileTitle, TEXT("Rebuild_"));
	_tcscat_s(szFilenameToSave, _tcsrchr(pThis->m_Filename, TEXT('\\')) + 1);

	//弹出选项对话框
	//调用对话框
	BOOL  bNeedRecompress;
	TRebuildOptDlg	dlg(pThis->m_PckHandle, &bNeedRecompress, pThis);
	//DWORD dwCompressLevel = pck_getCompressLevel(pThis->m_PckHandle);
	if (IDCANCEL == dlg.Exec())
		return;

	//选择保存的文件名
	int nSelectFilter = SaveFile(pThis->hWnd, szFilenameToSave, TEXT("pck"), pThis->BuildSaveDlgFilterString(), pck_getVersion(pThis->m_PckHandle));
	if (0 > nSelectFilter) {
		return;
	}

	pck_setVersion(pThis->m_PckHandle, nSelectFilter);

	//开始计时
	timer.start();

	pThis->EnbaleButtons(ID_MENU_REBUILD, FALSE);

	_stprintf_s(szPrintf, GetLoadStr(IDS_STRING_REBUILDING), _tcsrchr(szFilenameToSave, TEXT('\\')) + 1);
	pThis->SetStatusBarText(4, szPrintf);

	//pck_setThreadWorking(pThis->m_PckHandle);

	pThis->SetTimer(WM_TIMER_PROGRESS_100, 100, NULL);

	if (pck_RebuildPckFile(pThis->m_PckHandle, szFilenameToSave, bNeedRecompress)) {

		//计时结束
		timer.stop();

		if (pck_isLastOptSuccess(pThis->m_PckHandle)) {

			//pThis->m_PckLog.PrintLogN(GetLoadStr(IDS_STRING_REBUILDOK), timer.getElapsedTime());
			log_Print(LOG_IMAGE_NOTICE, GetLoadStr(IDS_STRING_REBUILDOK), timer.getElapsedTime());
		}
		else {
			pThis->SetStatusBarText(4, GetLoadStr(IDS_STRING_PROCESS_ERROR));
		}

	}
	else {

		pThis->SetStatusBarText(4, GetLoadStr(IDS_STRING_PROCESS_ERROR));

	}

	pThis->EnbaleButtons(ID_MENU_REBUILD, TRUE);

	if (bDeleteClass) {
		ListView_DeleteAllItems(pThis->GetDlgItem(IDC_LIST));
		pck_close(pThis->m_PckHandle);
	}

	pThis->KillTimer(WM_TIMER_PROGRESS_100);
	pThis->RefreshProgress();

	if ((!(pck_isLastOptSuccess(pThis->m_PckHandle))) && pThis->bGoingToExit) {
		pThis->bGoingToExit = FALSE;
		pThis->SendMessage(WM_CLOSE, 0, 0);
	}

	//pck_breakThreadWorking(pThis->m_PckHandle);

	return;

}

VOID TInstDlg::CreateNewPckFile(VOID	*pParam)
{
	TInstDlg	*pThis = (TInstDlg*)pParam;

	BOOL		isNotOpenedPck = !pck_IsValidPck(pThis->m_PckHandle);
	TCHAR		szFilenameToSave[MAX_PATH];

	TCHAR		szPrintf[64];

	CStopWatch	timer;

	//选择目录
	if (!OpenFilesVistaUp(pThis->hWnd, pThis->m_CurrentPath))
		return;

	pThis->m_lpszFilePath.push_back(pThis->m_CurrentPath);

	GetPckFileNameBySource(szFilenameToSave, pThis->m_CurrentPath, TRUE);

	//选择保存的文件名
	int nSelectFilter = SaveFile(pThis->hWnd, szFilenameToSave, TEXT("pck"), pThis->BuildSaveDlgFilterString());
	if (0 > nSelectFilter)
		return;

	//设定目标pck的版本
	pck_setVersion(pThis->m_PckHandle, nSelectFilter);

	//开始计时
	timer.start();

	pThis->EnbaleButtons(ID_MENU_NEW, FALSE);

	_stprintf_s(szPrintf, GetLoadStr(IDS_STRING_COMPING), _tcsrchr(szFilenameToSave, TEXT('\\')) + 1);
	pThis->SetStatusBarText(4, szPrintf);

	//pck_setThreadWorking(pThis->m_PckHandle);

	pThis->SetTimer(WM_TIMER_PROGRESS_100, TIMER_PROGRESS, NULL);

	pck_StringArrayReset(pThis->m_PckHandle);
	for (int i = 0; i < pThis->m_lpszFilePath.size(); i++) {
		pck_StringArrayAppend(pThis->m_PckHandle, pThis->m_lpszFilePath[i].c_str());
	}

	if (pck_UpdatePckFileSubmit(pThis->m_PckHandle, szFilenameToSave, pThis->m_currentNodeOnShow)) {

		//计时结束
		timer.stop();

		if (pck_isLastOptSuccess(pThis->m_PckHandle)) {

			//pThis->m_PckLog.PrintLogN(GetLoadStr(IDS_STRING_COMPOK), timer.getElapsedTime());
			log_Print(LOG_IMAGE_NOTICE, GetLoadStr(IDS_STRING_REBUILDOK), timer.getElapsedTime());
		}
		else {
			pThis->SetStatusBarText(4, GetLoadStr(IDS_STRING_PROCESS_ERROR));
		}

	}
	else {

		pThis->SetStatusBarText(4, GetLoadStr(IDS_STRING_PROCESS_ERROR));
	}

	pThis->EnbaleButtons(ID_MENU_NEW, TRUE);

	if (isNotOpenedPck)
		pck_close(pThis->m_PckHandle);

	pThis->KillTimer(WM_TIMER_PROGRESS_100);
	pThis->RefreshProgress();

	if ((!(pck_isLastOptSuccess(pThis->m_PckHandle))) && pThis->bGoingToExit) {

		pThis->bGoingToExit = FALSE;
		pThis->SendMessage(WM_CLOSE, 0, 0);
	}

	//pck_breakThreadWorking(pThis->m_PckHandle);

	return;

}

VOID TInstDlg::ToExtractAllFiles(VOID	*pParam)
{
	TInstDlg	*pThis = (TInstDlg*)pParam;
	TCHAR		szPrintf[64];

	CStopWatch	timer;

	//开始计时
	timer.start();

	pThis->EnbaleButtons(ID_MENU_UNPACK_ALL, FALSE);

	_stprintf_s(szPrintf, GetLoadStr(IDS_STRING_EXPING), _tcsrchr(pThis->m_Filename, TEXT('\\')) + 1);
	pThis->SetStatusBarText(4, szPrintf);

	//pck_setThreadWorking(pThis->m_PckHandle);

	pThis->SetTimer(WM_TIMER_PROGRESS_100, TIMER_PROGRESS, NULL);

	if (pck_ExtractAllFiles(pThis->m_PckHandle, pThis->m_CurrentPath)) {
		//计时结束
		timer.stop();
		_stprintf_s(szPrintf, GetLoadStr(IDS_STRING_EXPOK), timer.getElapsedTime());

		pThis->SetStatusBarText(4, szPrintf);

	}
	else {
		pThis->SetStatusBarText(4, GetLoadStr(IDS_STRING_PROCESS_ERROR));
	}

	pThis->EnbaleButtons(ID_MENU_UNPACK_ALL, TRUE);

	pThis->KillTimer(WM_TIMER_PROGRESS_100);
	pThis->RefreshProgress();

	if ((!(pck_isLastOptSuccess(pThis->m_PckHandle))) && pThis->bGoingToExit) {
		pThis->bGoingToExit = FALSE;
		pThis->SendMessage(WM_CLOSE, 0, 0);
	}

	//pck_breakThreadWorking(pThis->m_PckHandle);

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
			TCHAR		szPrintf[64];

			//取lpNodeToShow
			int	nCurrentItemCount = ListView_GetItemCount(hList);

			LVITEM item;
			item.mask = LVIF_PARAM | LVIF_STATE;
			item.iSubItem = 0;
			item.stateMask = LVIS_SELECTED;		// get all state flags

			lpFileEntryArrayPtr = lpFileEntryArray;

			uiSelectCount = 0;

			//从1开始，跳过..目录
			for (item.iItem = 1; item.iItem < nCurrentItemCount; item.iItem++) {
				ListView_GetItem(hList, &item);

				if (item.state & LVIS_SELECTED) {
					*lpFileEntryArrayPtr = (LPPCK_UNIFIED_FILE_ENTRY)item.lParam;
					lpFileEntryArrayPtr++;

					uiSelectCount++;
				}
			}

			if (0 == uiSelectCount)return;

			//pck_setThreadWorking(pThis->m_PckHandle);

			//开始计时
			timer.start();

			pThis->EnbaleButtons(ID_MENU_UNPACK_SELECTED, FALSE);

			_stprintf_s(szPrintf, GetLoadStr(IDS_STRING_EXPING), _tcsrchr(pThis->m_Filename, TEXT('\\')) + 1);
			pThis->SetStatusBarText(4, szPrintf);

			pThis->SetTimer(WM_TIMER_PROGRESS_100, TIMER_PROGRESS, NULL);

			lpFileEntryArrayPtr = lpFileEntryArray;

			if (pck_ExtractFilesByEntrys(pThis->m_PckHandle, lpFileEntryArray, uiSelectCount, pThis->m_CurrentPath)) {
				//计时结束
				timer.stop();
				_stprintf_s(szPrintf, GetLoadStr(IDS_STRING_EXPOK), timer.getElapsedTime());

				pThis->SetStatusBarText(4, szPrintf);
			}
			else {
				pThis->SetStatusBarText(4, GetLoadStr(IDS_STRING_PROCESS_ERROR));
			}

			free(lpFileEntryArray);

			pThis->EnbaleButtons(ID_MENU_UNPACK_SELECTED, TRUE);

			pThis->KillTimer(WM_TIMER_PROGRESS_100);
			pThis->RefreshProgress();

			if ((!(pck_isLastOptSuccess(pThis->m_PckHandle))) && pThis->bGoingToExit) {
				pThis->bGoingToExit = FALSE;
				pThis->SendMessage(WM_CLOSE, 0, 0);
			}
		}
	}
	//pck_breakThreadWorking(pThis->m_PckHandle);

	return;
}


VOID TInstDlg::DeleteFileFromPckFile(VOID	*pParam)
{
	TInstDlg	*pThis = (TInstDlg*)pParam;

	HWND	hList = pThis->GetDlgItem(IDC_LIST);

	CStopWatch	timer;

	UINT uiSelectCount = ListView_GetSelectedCount(hList);

	if (0 != uiSelectCount) {

		TCHAR		szPrintf[64];

		int	nCurrentItemCount = ListView_GetItemCount(hList);

		LVITEM item;
		item.mask = LVIF_PARAM | LVIF_STATE;
		item.iSubItem = 0;
		item.stateMask = LVIS_SELECTED;		// get all state flags

		uiSelectCount = 0;

		for (item.iItem = 1; item.iItem < nCurrentItemCount; item.iItem++) {
			ListView_GetItem(hList, &item);

			if (item.state & LVIS_SELECTED) {
				pck_DeleteEntry(pThis->m_PckHandle, (LPPCK_UNIFIED_FILE_ENTRY)item.lParam);
				uiSelectCount++;
			}
		}

		if (0 == uiSelectCount)return;

		//开始计时
		timer.start();

		//pck_setThreadWorking(pThis->m_PckHandle);

		pThis->EnbaleButtons(ID_MENU_DELETE, FALSE);

		_stprintf_s(szPrintf, GetLoadStr(IDS_STRING_RENEWING), _tcsrchr(pThis->m_Filename, TEXT('\\')) + 1);
		pThis->SetStatusBarText(4, szPrintf);

		pThis->SetTimer(WM_TIMER_PROGRESS_100, TIMER_PROGRESS, NULL);


		if (pck_RenameSubmit(pThis->m_PckHandle)) {
			//计时结束
			timer.stop();
			_stprintf_s(szPrintf, GetLoadStr(IDS_STRING_RENEWOK), timer.getElapsedTime());

			pThis->SetStatusBarText(4, szPrintf);

			pThis->OpenPckFile(pThis->m_Filename, TRUE);

		}
		else {
			pThis->SetStatusBarText(4, GetLoadStr(IDS_STRING_PROCESS_ERROR));
		}

		pThis->EnbaleButtons(ID_MENU_DELETE, TRUE);

		pThis->KillTimer(WM_TIMER_PROGRESS_100);
		pThis->RefreshProgress();

		if (pThis->bGoingToExit) {
			pThis->bGoingToExit = FALSE;
			pThis->SendMessage(WM_CLOSE, 0, 0);
		}

	}
	//pck_breakThreadWorking(pThis->m_PckHandle);

	return;

}

//从拖入的源文件名推出预保存的pck文件名
VOID GetPckFileNameBySource(LPTSTR dst, LPCTSTR src, BOOL isDirectory)
{
	int szPathToCompressLen;
	_tcscpy(dst, src);

	if (isDirectory) {
		if ((szPathToCompressLen = lstrlen(dst)) > 13 && 0 == lstrcmp(dst + szPathToCompressLen - 10, TEXT(".pck.files"))) {
			*(dst + szPathToCompressLen - 10) = 0;
		}
	}

	_tcscat(dst, TEXT(".pck"));
}