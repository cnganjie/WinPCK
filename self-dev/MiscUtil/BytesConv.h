#pragma once
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
