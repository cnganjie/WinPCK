#pragma once

#include "WinControl.h"
#include <memory>


class CEdit :
	public CWinControl
{
public:
	CEdit() = default;
	virtual ~CEdit() = default;

	virtual void Attach(int id, HWND hDlg) override;
	virtual void SetLimitText(int size = -1);
	virtual int GetLimitText();


	virtual void SetText(const char* str) override;
	virtual void SetText(const wchar_t* str) override;

	//����
	
	/// <summary>
	/// 
	/// </summary>
	/// <param name="line">��0��ʼ</param>
	/// <returns>��line�еĵ�һ���ַ�������</returns>
	virtual int LineIndex(int line);
	virtual void SetSel(int start, int end);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="line">��0��ʼ</param>
	virtual void SetSelLine(int line);
	virtual void SetSelLine(int start, int end);

	virtual void SetSelEnd() {
		this->SetSel(-2, -1);
	}

	virtual void SetSelAll() {
		this->SetSel(0, -1);
	}

	virtual uint32_t GetLineCount();

	//�ı�
	virtual void SetSelText(const char* text);
	virtual void SetSelText(const char8_t* text);
	virtual void SetSelText(const wchar_t* text);

	template<typename T>
	void AppendText(const T* text)
	{
		auto scroll = this->IsVScrollOnBottom();
		this->SetSelEnd();
		this->SetSelText(text);
		if (scroll)
			this->SendMsgA(WM_VSCROLL, SB_BOTTOM, 0);
	}

	virtual void DeleteLine(int start, int end);

};
