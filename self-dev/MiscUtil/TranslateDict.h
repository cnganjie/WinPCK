#pragma once
//////////////////////////////////////////////////////////////////////
// TranslateDict.h
// 
//
// �˳����� �����/stsm/liqf ��д
//
// �˴���Ԥ�ƽ��ῪԴ���κλ��ڴ˴�����޸ķ����뱣��ԭ������Ϣ
// 
// 2024.1.1
//////////////////////////////////////////////////////////////////////


#include <string>
#include <unordered_map>
#include "json.hpp"

class CTranslateDict
{
private:
	CTranslateDict() = default;
	CTranslateDict(const CTranslateDict&) = delete;
	const CTranslateDict& operator=(const CTranslateDict&) = delete;
	virtual ~CTranslateDict() = default;


public:

	static CTranslateDict& GetInstance() {
		static CTranslateDict onlyInstance;
		return onlyInstance;
	}

	//bool add(const std::string& laug_name, const std::string& json_str);
	bool addfile(const std::string& filename);

	const std::wstring& getTransString(const std::string& id);

private:

	std::map<std::string, int> m_language_list;
	std::vector<std::unordered_map<std::string, std::wstring>> m_language_map;

};
