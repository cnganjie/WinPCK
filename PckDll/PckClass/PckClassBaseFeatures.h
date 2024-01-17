#pragma once
#include "PckHeader.h"
#include "PckClassLog.h"

#include <MemPool.h>
#include "PckClassZlib.h"


class CPckClassBaseFeatures 
{
public:
	CPckClassBaseFeatures();
	~CPckClassBaseFeatures();

	virtual const	LPPCKINDEXTABLE		GetPckIndexTable();
	virtual const	LPPCK_PATH_NODE		GetPckPathNode();

	//�Ƿ��Ѿ������ļ�
	BOOL	isFileLoaded();

	//�ļ���С
	uint64_t	GetPckSize();

	//��ȡ�ļ���
	uint32_t	GetPckFileCount();

	//��������С
	uint64_t	GetPckDataAreaSize();

	//�������������ݴ�С
	uint64_t	GetPckRedundancyDataSize();

protected:
	//������Ϣ
	void	ResetPckInfos();

	//���ý���
	void	SetParams_ProgressInc();
	void	SetParams_Progress(DWORD dwUIProgres);

	//���ý�����Ϣ�е����ֵ
	void	SetParams_ProgressUpper(DWORD dwUIProgressUpper, BOOL bReset = TRUE);
	void	AddParams_ProgressUpper(DWORD dwUIProgressUpperAdd);

	//���̴߳������
	void	SetThreadFlag(BOOL isThreadWorking);
	BOOL	CheckIfNeedForcedStopWorking();
	void	SetErrMsgFlag(int errMsg);

	PCK_ALL_INFOS			m_PckAllInfo;

	CAllocMemPool			m_NodeMemPool;

	CPckClassZlib			m_zlib;

	//����ʱ���� 
	LPPCK_RUNTIME_PARAMS	m_lpPckParams;

};
