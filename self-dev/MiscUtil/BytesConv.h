#pragma once
//////////////////////////////////////////////////////////////////////
// BytesConv.h
// 
//
// �˳����� �����/stsm/liqf ��д
//
// �˴���Ԥ�ƽ��ῪԴ���κλ��ڴ˴�����޸ķ����뱣��ԭ������Ϣ
// 
// 2024.1.1
//////////////////////////////////////////////////////////////////////

#include <stdint.h>
#include <string>
#include <format>

/// <summary>
/// �ֽ���ת��Ϊ KB MB GB TB....
/// </summary>
/// <param name="bytes"></param>
/// <returns></returns>

std::string byte_format(uint64_t bytes);
std::wstring wbyte_format(uint64_t bytes);
