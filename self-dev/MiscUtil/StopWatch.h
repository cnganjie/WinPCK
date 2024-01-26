#pragma once
//////////////////////////////////////////////////////////////////////
// StopWatch.h
// 
//
// �˳����� �����/stsm/liqf ��д
//
// �˴���Ԥ�ƽ��ῪԴ���κλ��ڴ˴�����޸ķ����뱣��ԭ������Ϣ
// 
// 2024.1.1
//////////////////////////////////////////////////////////////////////


#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ�ļ����ų�����ʹ�õ�����
#include <Windows.h>

#ifdef UNICODE 
#define getElapsedTimeString getElapsedTimeStringW
#else
#define getElapsedTimeString getElapsedTimeStringA
#endif

class CStopWatch
{
public:
	CStopWatch();
	void start();
	void stop();
	double getElapsedTime(); //in s
	//const char * getElapsedTimeStringA();
	//const wchar_t * getElapsedTimeStringW();

private:
	LARGE_INTEGER m_start;
	LARGE_INTEGER m_stop;
	LARGE_INTEGER m_frequency;
};
