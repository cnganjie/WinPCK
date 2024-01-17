#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

/// <summary>
/// ����ļ����Ƿ���д������Ȩ��
/// </summary>
/// <param name="strPath"></param>
/// <param name="dwGenericAccessMask">GENERIC_READ, GENERIC_WRITE, GENERIC_EXECUTE, GENERIC_ALL</param>
/// <returns></returns>
template<typename T>
BOOL CanAccessFile(const T* strPath, DWORD dwGenericAccessMask);

