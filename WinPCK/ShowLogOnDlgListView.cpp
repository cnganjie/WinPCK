//////////////////////////////////////////////////////////////////////
// ShowLogOnDlgListView.cpp: ����־��ʾ��listView��
// 
//
// �˳����� �����/stsm/liqf ��д
//
// �˴���Ԥ�ƽ��ῪԴ���κλ��ڴ˴�����޸ķ����뱣��ԭ������Ϣ
// 
// 2018.6.4
//////////////////////////////////////////////////////////////////////

#include "guipch.h"
#include "ShowLogOnDlgListView.h"


CLogUnits LogUnits;

void	PreInsertLogToList(const char ch, const wchar_t *str)
{
	LogUnits.InsertLog(ch, str);
}


CLogUnits::CLogUnits()
{
}

CLogUnits::~CLogUnits()
{
}
