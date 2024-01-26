#pragma once
//////////////////////////////////////////////////////////////////////
// TextEncoding.h
// 
//
// �˳����� �����/stsm/liqf ��д
//
// �˴���Ԥ�ƽ��ῪԴ���κλ��ڴ˴�����޸ķ����뱣��ԭ������Ϣ
// 
// 2024.1.1
//////////////////////////////////////////////////////////////////////

namespace TextEncoding
{
	enum class EncodingType
	{
		ANSI,
		UCS2,
		UTF8,
		RAW
	};

	EncodingType	TextDataType(const char*& _s, size_t size);
}

