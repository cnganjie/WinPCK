//////////////////////////////////////////////////////////////////////
// mainControlStatus.cpp: WinPCK �����̲߳���
// ����ؼ�����
//
// �˳����� �����/stsm/liqf ��д
//
// �˴��뿪Դ���κλ��ڴ˴�����޸ķ����뱣��ԭ������Ϣ
// 
// 2019.9.10
//////////////////////////////////////////////////////////////////////

#include "guipch.h"
#include "winmain.h"

void TInstDlg::SetStatusBarText(int	iPart, LPCWSTR	lpszText)
{
	this->SendDlgItemMessageW(IDC_STATUS, SB_SETTEXTW, iPart, (LPARAM)lpszText);
	this->SendDlgItemMessageW(IDC_STATUS, SB_SETTIPTEXTW, iPart, (LPARAM)lpszText);
}

void TInstDlg::SetStatusBarTitle(LPCWSTR lpszText)
{
	this->SetStatusBarText(0, lpszText);
}

void TInstDlg::SetStatusBarFileSize(uint64_t size)
{
	this->SetStatusBarText(1, std::vformat(::GetLoadStrW(IDS_STRING_OPENFILESIZE), std::make_wformat_args(size)).c_str());
}

void TInstDlg::SetStatusBarFileCount(uint32_t size)
{
	this->SetStatusBarText(2, std::vformat(::GetLoadStrW(IDS_STRING_OPENFILECOUNT), std::make_wformat_args(size)).c_str());
}

void TInstDlg::ClearStatusBarProgress()
{
	this->SetStatusBarText(3, L"");
}

void TInstDlg::SetStatusBarProgress(LPCWSTR lpszText)
{
	this->SetStatusBarText(3, lpszText);
}

void TInstDlg::SetStatusBarInfo(LPCWSTR lpszText)
{
	this->SetStatusBarText(4, lpszText);
}