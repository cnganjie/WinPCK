#pragma once

#include <string>
#include <format>

#include "StackWalker.h"

/// <summary>
/// ����������������ɵ��ö�ջ�б����Ϣ
/// </summary>

class StackWalkerLiqf : public StackWalker
{
public:
	StackWalkerLiqf(const wchar_t* file);
	virtual ~StackWalkerLiqf();

private:
	virtual void OnOutput(LPCSTR szText);

	HANDLE		hFile;
	SYSTEMTIME	tm;
};

class StackWalkerMem : public StackWalker
{
public:
	StackWalkerMem() {
		this->buffer.reserve(32*1024);
	};
	virtual ~StackWalkerMem() = default;

	std::string& text()
	{
		return this->buffer;
	}

private:
	virtual void OnOutput(LPCSTR szText);

	std::string buffer;

};
