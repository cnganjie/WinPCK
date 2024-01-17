//////////////////////////////////////////////////////////////////////
// ShowPicture.cpp: ��ʾdds��tgaͼ��
// ����dds��tga����ʾ
//
// �˳����� �����/stsm/liqf ��д
//
// �˴���Ԥ�ƽ��ῪԴ���κλ��ڴ˴�����޸ķ����뱣��ԭ������Ϣ
// 
// 2018.5.29
//////////////////////////////////////////////////////////////////////

#include "showimgpch.h"
#include "ShowPicture.h"

#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#pragma warning ( disable : 4018 )
#pragma warning ( disable : 4996 )

CShowPicture::CShowPicture()
{
	Gdiplus::GdiplusStartup(&this->m_pGdiPlusToken, &this->m_oGdiPlusStartupInput, NULL);
}

CShowPicture::~CShowPicture()
{
	if(NULL != this->m_lpmyImage)
		delete this->m_lpmyImage;

	if(NULL != this->m_stream)
		this->m_stream->Release();

	if(FMT_RAW != this->m_picFormat) {
		if(NULL != this->m_resBufferInClass) {
			free(this->m_resBufferInClass);
		}
	} else {

		if(NULL != this->m_hGlobal)
			GlobalFree(this->m_hGlobal);
	}

	if(NULL != this->m_DecodedDIBData) {
		free(this->m_DecodedDIBData);
	}

	DeleteObject(this->m_MemBitmap);
	DeleteDC(this->m_MemDC);

	Gdiplus::GdiplusShutdown(this->m_pGdiPlusToken);
}

bool CShowPicture::Attach(HWND hWndShow, LPBYTE _buffer, size_t _bufsize, LPCWSTR _lpszFileTitle, PICFORMAT _picFormat)
{
	this->m_hWndShow = hWndShow;
	this->m_lpszFileTitle = _lpszFileTitle;
	this->m_picFormat = _picFormat;
	this->m_bufsize = _bufsize;

	if (0 == _bufsize)
		return false;

	if (FMT_RAW != (this->m_picFormat))	//dds,tga
	{
		this->m_resBufferInClass = (LPBYTE)malloc(this->m_bufsize);
	}
	else {							//bmp,jpg,png..
		this->m_resBufferInClass = (LPBYTE)(this->m_hGlobal = GlobalAlloc(GMEM_FIXED, m_bufsize));
	}

	if (nullptr != this->m_resBufferInClass) {
		//��������
		memcpy(this->m_resBufferInClass, _buffer, _bufsize);
	}

	return this->Decode();
}

_inline BOOL CShowPicture::isEqual(double d1, double d2)
{
	return (EPSILON > fabs((d1-d2)));
}

std::wstring CShowPicture::GetWindowTitle() const
{
	std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>> 
		converter(new std::codecvt<wchar_t, char, std::mbstate_t>("chs"));

	//��������
	if(FMT_RAW != this->m_picFormat) {
		return std::format(L"ͼƬ�鿴 - {}, {}x{}, {}", this->m_lpszFileTitle, this->m_picWidth, this->m_picHeight, converter.from_bytes(this->m_szPictureFormat));
	} else {
		return std::format(L"ͼƬ�鿴 - {}, {}x{}", this->m_lpszFileTitle, this->m_picWidth, this->m_picHeight);
	}
}

//����ͼ��Ŀ�͸�
uint32_t CShowPicture::GetWidth() const
{
	return m_picWidth;
}

uint32_t CShowPicture::GetHeight() const
{
	return m_picHeight;
}


void CalcZoomInAreaAndDrawPosition(int64_t& dst_xy, int64_t& dst_wh, int64_t& src_xy, int64_t& src_wh, double dZoomRatio, LONG iClientWidthHeight, UINT uiPicWidthHeight)
{
	/*
	�����ź�ͼ����ڴ��ں��趨�������Ͻ�����Ϊ 0,0
	���δ��ʾ���������趨Ϊleft_invisible(����ֵ),�Ҷ�Ϊright_invisible����������ΪRatio��
	�����δ��ʾ��ʵ��ͼ���С����Ϊ left_invisible_real = (int)left_invisible/Ratio������Ϊleft_invisible_rem
	�Ҳ���ͬΪright_invisible = tmp_dst_w - left_invisible - rectDlg.right��right_invisible_real = (int)right_invisible/Ratio��right_invisible_rem
	ͬʱ����������(left_invisible_rem)ΪҪȡ���������ź���ʾ�ڴ����ϵ�ƫ��
	��ʵ��ͼ����Ϊm_picWidthʵ��Ҫȡ������Ϊ��visible_width_real = m_picWidth - left_invisible_real - right_invisible_real,
	Ŀ�����Ͻ���ʾ����dst_x = left_invisible_rem
	Դ���Ͻ�X����src_x = left_invisible_real
	*/

	int64_t		left_invisible = -dst_xy;
	double	left_invisible_real_double = left_invisible / dZoomRatio;
	int64_t		left_invisible_real = (int64_t)left_invisible_real_double;
	int64_t		left_invisible_rem = (int64_t)(left_invisible - left_invisible_real * dZoomRatio);
	int64_t		right_invisible = dst_wh - left_invisible - iClientWidthHeight;
	int64_t		right_invisible_real_double = (int64_t)(right_invisible / dZoomRatio);
	int64_t		right_invisible_real = (int64_t)right_invisible_real_double;
	int64_t		right_invisible_rem = (int64_t)(right_invisible - right_invisible_real * dZoomRatio);
	int64_t		visible_width_real = uiPicWidthHeight - left_invisible_real - right_invisible_real;

	dst_xy = -left_invisible_rem;
	src_xy = left_invisible_real;
	src_wh = visible_width_real;
	dst_wh = iClientWidthHeight + right_invisible_rem + left_invisible_rem;

}

//���豸����ʾͼ��
bool CShowPicture::Paint(int64_t nXOriginDest, int64_t nYOriginDest, int nXOriginSrc, int nYOriginSrc, double dZoomRatio, bool bErase)
{
	/*
	�趨�������Ͻ�����Ϊ 0,0
	pic_x:����ʾͼ�� �����Ͻ�����ڴ��ڵ�λ��
	*/
	RECT rectDlg;
	::GetClientRect(this->m_hWndShow, &rectDlg);

	PAINTSTRUCT ps;
	HDC hDC = ::BeginPaint(this->m_hWndShow, &ps);

	//ֻ��ʹ��GetDC���ܼ�ʱˢ��
	HDC pDC = ::GetDC(this->m_hWndShow);

	if (this->IsDecoded()) {

		if (bErase)
			RedrawInvalidRect(pDC, rectDlg, nXOriginDest, nYOriginDest, dZoomRatio);

		DrawImage(pDC, rectDlg, ps, nXOriginDest, nYOriginDest, nXOriginSrc, nYOriginSrc, dZoomRatio);
	}
	else {
		if (bErase)
			::FillRect(pDC, &rectDlg, this->m_hBrushBack);
	}

	::ReleaseDC(this->m_hWndShow, pDC);
	::EndPaint(this->m_hWndShow, &ps);
	return true;
}

//����ͼ��
void CShowPicture::DrawImage(HDC& hDC, RECT& rectDlg, PAINTSTRUCT& ps, int64_t nXOriginDest, int64_t nYOriginDest, int nXOriginSrc, int nYOriginSrc, double dZoomRatio)
{
	//�ؼ���Դ��㡢��ߺ�Ŀ����㡢���
	int64_t src_x = nXOriginSrc;
	int64_t src_y = nYOriginSrc;
	int64_t src_w = this->m_picWidth;
	int64_t src_h = this->m_picHeight;
	int64_t dst_x = nXOriginDest;
	int64_t dst_y = nYOriginDest;
	int64_t dst_w = this->m_picWidth;
	int64_t dst_h = this->m_picHeight;

	if (isEqual(dZoomRatio, 1.0)) {
		//������
		if (nXOriginDest < 0) {

			dst_x = 0;
			src_x = nXOriginSrc - nXOriginDest;
			dst_w = (((int64_t)this->m_picWidth - src_x) > (int64_t)rectDlg.right) ? (int64_t)rectDlg.right : ((int64_t)this->m_picWidth - src_x);
		}
		if (nYOriginSrc < 0) {

			dst_y = 0;
			src_y = nYOriginSrc - nYOriginDest;
			dst_h = (((int64_t)this->m_picHeight - src_y) > (int64_t)rectDlg.bottom) ? (int64_t)rectDlg.bottom : ((int64_t)this->m_picHeight - src_y);
		}

		::BitBlt(
			hDC,				// Ŀ�� DC ���
			(int)dst_x,				// Ŀ�����Ͻ�X����
			(int)dst_y,				// Ŀ�����Ͻ�Y����
			(int)dst_w,				// Ŀ����
			(int)dst_h,				// Ŀ��߶�
			this->m_MemDC, 			// Դ DC ���
			(int)src_x,				// Դ���Ͻ�X����
			(int)src_y,				// Դ���Ͻ�Y����
			SRCCOPY);

	} else {
		//����
		dst_w = (int64_t)(this->m_picWidth * dZoomRatio + 0.5);
		dst_h = (int64_t)(this->m_picHeight * dZoomRatio + 0.5);

		if (rectDlg.right < dst_w)
			CalcZoomInAreaAndDrawPosition(dst_x, dst_w, src_x, src_w, dZoomRatio, rectDlg.right, this->m_picWidth);

		if (rectDlg.bottom < dst_h)
			CalcZoomInAreaAndDrawPosition(dst_y, dst_h, src_y, src_h, dZoomRatio, rectDlg.bottom, this->m_picHeight);

		//��ͼƬΪ��Сʱ��ʹ��STRETCH_HALFTONE(��������)���Ŵ�ʱ��ʹ��STRETCH_DELETESCANS
		if (1.0 > dZoomRatio)
			::SetStretchBltMode(hDC, STRETCH_HALFTONE);
		else
			::SetStretchBltMode(hDC, this->m_upscale_mode);

		::StretchBlt(
			hDC,				// Ŀ�� DC ���
			(int)dst_x,				// Ŀ�����Ͻ�X����
			(int)dst_y,				// Ŀ�����Ͻ�Y����
			(int)dst_w,				// Ŀ����
			(int)dst_h,				// Ŀ��߶�
			this->m_MemDC,			// Դ DC ���
			(int)src_x,				// Դ���Ͻ�X����
			(int)src_y,				// Դ���Ͻ�Y����
			(int)src_w,				// Դ���
			(int)src_h,				// Դ�߶�
			SRCCOPY);
	}
}

//������Ч����
void CShowPicture::RedrawInvalidRect(HDC& hdc, RECT& rectWin, int64_t nXOriginDest, int64_t nYOriginDest, double dZoomRatio)
{
	uint64_t dst_width = this->m_picWidth * dZoomRatio;
	uint64_t dst_height = this->m_picHeight * dZoomRatio;

	if (nXOriginDest > 0) {

		RECT rectInvalid = { 0, 0, nXOriginDest, rectWin.bottom };
		::FillRect(hdc, &rectInvalid, this->m_hBrushBack);
	}

	if (nYOriginDest > 0) {

		RECT rectInvalid = { nXOriginDest > 0 ? nXOriginDest : 0, 0, rectWin.right, nYOriginDest };
		::FillRect(hdc, &rectInvalid, this->m_hBrushBack);
	}

	int iShowBottom = nYOriginDest + dst_height;

	if (iShowBottom < rectWin.bottom) {

		RECT rectInvalid = { nXOriginDest > 0 ? nXOriginDest : 0, iShowBottom, rectWin.right, rectWin.bottom };
		::FillRect(hdc, &rectInvalid, this->m_hBrushBack);
	}

	int iShowRight = nXOriginDest + dst_width;
	if (iShowRight < rectWin.right) {

		RECT rectInvalid = { iShowRight, nYOriginDest > 0 ? nYOriginDest : 0,rectWin.right, iShowBottom < rectWin.bottom ? iShowBottom : rectWin.bottom };
		::FillRect(hdc, &rectInvalid, this->m_hBrushBack);
	}
}

bool CShowPicture::Decode()
{
	this->m_isDecoded = false;

	if(NULL == this->m_resBufferInClass)
		return false;

	if (!this->BuildImage())
		return false;

	if(nullptr != this->m_hWndShow)
		if (!this->DrawPictureOnDC())
		return false;

	this->m_isDecoded = true;
	return true;
}