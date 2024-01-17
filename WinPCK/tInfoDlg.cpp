//////////////////////////////////////////////////////////////////////
// tInfoDlg.cpp: WinPCK �����̲߳���
// �Ի������
//
// �˳����� �����/stsm/liqf ��д
//
// �˴���Ԥ�ƽ��ῪԴ���κλ��ڴ˴�����޸ķ����뱣��ԭ������Ϣ
// 
// 2017.12.26
//////////////////////////////////////////////////////////////////////

#include "guipch.h"
#include "tInfoDlg.h"

/*
������Ϣ�Ի���
*/
BOOL TInfoDlg::EvCreate(LPARAM lParam)
{
	this->SendDlgItemMessage(IDC_EDIT_INFO, EM_LIMITTEXT, pck_GetAdditionalInfoMaxSize() - 1, 0);
	this->SetDlgItemTextA(IDC_EDIT_INFO, pck_GetAdditionalInfo());
	return	TRUE;
}

BOOL TInfoDlg::EvCommand(WORD wNotifyCode, WORD wID, LPARAM hwndCtl)
{
	switch(wID) {
	case IDOK:

		this->set_additional_info();
		this->EndDialog(wID);
		return	TRUE;

	case IDCANCEL:
		this->EndDialog(wID);
		return	TRUE;
	}
	return	FALSE;
}

void TInfoDlg::set_additional_info()
{
	auto szAdditionalInfo = std::make_unique<char[]>(pck_GetAdditionalInfoMaxSize());
	memset(szAdditionalInfo.get(), 0, pck_GetAdditionalInfoMaxSize());
	this->GetDlgItemTextA(IDC_EDIT_INFO, szAdditionalInfo.get(), pck_GetAdditionalInfoMaxSize());

	//����1Ϊ�����ɹ���success=WINPCK_OK
	pck_SetAdditionalInfo(szAdditionalInfo.get());
}
