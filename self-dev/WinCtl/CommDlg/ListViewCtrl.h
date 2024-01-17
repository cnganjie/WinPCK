#pragma once
#include <Windows.h>
#include <CommCtrl.h>

#include <vector>
#include <string>


template<typename T>
struct INIT_LIST_PARAMS {
	//����
	T colTitle;
	//���
	int cx;
	//���뷽ʽ
	int ifmt;
};

class CListViewCtrl
{
public:
	CListViewCtrl(HWND hWnd);
	~CListViewCtrl();

#pragma region ��ʼ��

public:
	BOOL InitListView(std::vector<INIT_LIST_PARAMS<std::string>> cols, std::vector<int> icons = std::vector<int>());
	BOOL InitListView(std::vector<INIT_LIST_PARAMS<std::wstring>> cols, std::vector<int> icons = std::vector<int>());

private:
	template<typename T, typename T_LVCOL, int T_LVM_INSERTCOL>
	BOOL InitLVColsTemplate(std::vector<INIT_LIST_PARAMS<T>> cols, std::vector<int> icons);

#pragma endregion

#pragma region �������

public:
	template<typename T>
	void AppendListWithImage(CONST INT iImage, CONST LPVOID lParam, std::vector<T> items) const;
	template<typename T>
	void AppendListWithOutImage(CONST LPVOID lParam, std::vector<T> items) const;

	void AppendList(CONST UINT uiMask, CONST INT iImage, CONST LPVOID lParam, std::vector<std::string> items) const;
	void AppendList(CONST UINT uiMask, CONST INT iImage, CONST LPVOID lParam, std::vector<std::wstring> items) const;



private:
	template<typename T, typename T_LVITEM, int T_LVM_INSERTITEM, int T_LVM_SETITEM,
		template<typename VELEM, typename = std::char_traits<VELEM>, typename = std::allocator<VELEM>> class TSTRING = std::basic_string>
	void InsertList(CONST INT iItem, CONST UINT uiMask, CONST INT iImage, CONST LPVOID lParam, std::vector<TSTRING<T>>& items) const;

#pragma endregion

#pragma region ɾ������
public:
	void DeleteItem(const int iItem);
	void DeleteAllItems();

#pragma endregion


#pragma region ��ȡ�޸Ĳ���

public:

	void SetItemHeight(int cx);

	LPARAM GetListParam(const int index);

	int GetItemCount();
	std::vector<int> GetSelectedItem();

	int GetListIcon(const int index);
	void SetListIcon(const int index, const int iIcon);

	void SetListText(const int iItem, std::vector<int> iSubItems, std::vector<std::string> items) const;
	void SetListText(const int iItem, std::vector<int> iSubItems, std::vector<std::wstring> items) const;

private:
	template<typename T, typename T_LVITEM, int T_LVM_SETITEM,
		template<typename VELEM, typename = std::char_traits<VELEM>, typename = std::allocator<VELEM>> class TSTRING = std::basic_string>
	void SetListItemText(CONST INT iItem, std::vector<int> iSubItems, std::vector<TSTRING<T>>& items) const;
#pragma endregion

#pragma region �ж�����

public:

	bool isItemChangedToSelected(LPNMLISTVIEW pNMLV);

	bool isStatChanged(const int changed);
	bool isFocusedStat(const int stat);
	bool isSelectedStat(const int stat);



#pragma endregion


private:

	HWND m_hWnd;
	HIMAGELIST m_hSmall = nullptr;   // image list for other views 
	HINSTANCE m_hInstance = ::GetModuleHandle(0);

	bool isInited = false;
	int m_item_height = 24;
};


