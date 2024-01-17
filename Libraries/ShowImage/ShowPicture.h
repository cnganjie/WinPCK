#pragma once

#define EPSILON 0.000001

#include "ShowPictureInterface.h"

class CShowPicture : public IShowPicture
{
public:
	CShowPicture();
	virtual ~CShowPicture();

	virtual bool	Attach(HWND hWndShow, LPBYTE _buffer, size_t _bufsize, LPCWSTR _lpszFileTitle = L"", PICFORMAT _picFormat = FMT_RAW) override;
	virtual void	Detach() override {};

	//����ͼ��Ŀ�͸�
	virtual uint32_t	GetWidth() const override;
	virtual uint32_t	GetHeight() const override;

	//
	virtual bool	IsDecoded() const override { return this->m_isDecoded; }

	void	SetHQUpscaleMode(bool isHQ = true)
	{
		this->m_upscale_mode = isHQ ? STRETCH_HALFTONE : STRETCH_DELETESCANS;
	}

#pragma region Window
public:
	virtual std::wstring	GetWindowTitle() const override;

#pragma endregion


#pragma region DrawPicture

protected:
	//��m_lpmyImage�����ϴ���ͼ��image
	BOOL	BuildImage();
	//��Graphicsͼ���ϻ�ͼ
	BOOL	DrawPictureOnDC();
	//�ڴ���������ʾ��͸�������ĸ���
	BOOL	DrawBlockOnDlg();
#pragma endregion

public:

	//���豸����ʾͼ��
	virtual bool	Paint(int64_t nXOriginDest, int64_t nYOriginDest, int nXOriginSrc, int nYOriginSrc, double dZoomRatio = 1.00, bool bErase = true) override;
private:

	//����ͼ��
	void	DrawImage(HDC& hDC, RECT& rectWin, PAINTSTRUCT& ps, int64_t nXOriginDest, int64_t nYOriginDest, int nXOriginSrc, int nYOriginSrc, double dZoomRatio);

	//������Ч����
	void	RedrawInvalidRect(HDC& hDC, RECT& rectWin, int64_t nXOriginDest, int64_t nYOriginDest, double dZoomRatio);

#pragma region ����ͼƬ
public:
	//������ʾ��ͼƬ
	virtual bool	Save(const wchar_t* lpszFilename) override;
#pragma endregion

	//�̳��಻�ɷ���


	//�̳���ɷ���
protected:

	//Gdiplus
	Gdiplus::GdiplusStartupInput m_oGdiPlusStartupInput;
	ULONG_PTR	m_pGdiPlusToken;

	//ͼƬ����dds,tga,other
	//enum { FMT_DDS, FMT_TGA, FMT_RAW} m_picFormat;
	PICFORMAT	m_picFormat;
	//������ļ��������Ը��ݺ�׺�ж�����
	LPCWSTR		m_lpszFileTitle;
	//�󶨵�������ʾ�Ĵ��ڵľ��
	HWND		m_hWndShow;
	//ͼƬ�Ŀ�͸�
	UINT		m_picWidth = 0, m_picHeight = 0;
	//��Ҫ��ʾ��ͼ��ử�����HDC����
	HDC			m_MemDC = NULL;
	//
	HBITMAP		m_MemBitmap = NULL;
	//�����ڴ�������ʾͼƬ��GDIPLUS����
	Gdiplus::Image* m_lpmyImage = NULL;
	//���ڲ�ʹ�õ�ԭʼ��ͼƬ�ļ����ݣ�������ΪRAWʱ��ָ��hGlobal���ڴ�
	LPBYTE		m_resBufferInClass = nullptr;
	//������ʾbmp,jpg,png��ͼƬ��ʹ��Gdiplus��hGlobal�ڴ洴����IStream s��Ȼ��ͼ
	IStream* m_stream = NULL;
	HGLOBAL		m_hGlobal = NULL;
	//������ļ������ݵĴ�С
	UINT32		m_bufsize;
	//��ʾ��ͼƬ�ľ����ʽ
	char		m_szPictureFormat[32] = { 0 };
	//�Ѿ������ͼƬ��DIB���ݣ�DDS��TGA��
	LPBYTE		m_DecodedDIBData = NULL;

	//���Ʊ���
	HBRUSH		m_hBrushBack = GetSysColorBrush(COLOR_MENU);

	//�Ƿ�ɹ�����ͼƬ
	bool		m_isDecoded = false;

	int m_upscale_mode = STRETCH_HALFTONE;

	//�ж�����double���
	BOOL	isEqual(double d1, double d2);

	//��ʼ����
	bool	Decode();

public:
	//���ŵ����ڴ�С
	virtual void	ZoomToWindowSize() override {};

	//�ָ�ԭʼ�ֱ���
	virtual void	ZoomToOriginalResolution(int64_t pos_x, int64_t pos_y) override {};

	//ʹ�������ֽ�������
	virtual void	ZoomWithMousePoint(uint32_t nFlags, int16_t zDelta, int64_t pos_x, int64_t pos_y) override {};

	//����������
	virtual double	GetZoomRatio() override {
		return 1;
	};

	//���ڴ�С�仯��Ҫ���¼���ֵ
	virtual void	ChangeClientSize(uint16_t nWidth, uint16_t nHeight) override {};
	virtual double	CalcFixedRatioByClientSize(uint32_t uRealX, uint32_t uRealY, uint32_t uToX, uint32_t uToY) override { return 0; };
	//���ڳ�ʼ��ʱ���������ֵ�����ͼƬ���ڷֱ��ʣ����ھ���󻯣�����ֵΪ�����Ƿ���Ҫ��󻯣���Ҫ����TRUE
	virtual bool	CalcFixedRatioAndSizeOnInit(int64_t& _inout_cx, int64_t& _inout_cy, int _in_min_cx, int _in_min_cy) override { return false; };

	//�ƶ�ͼ��
	virtual void	MovePicture(int xOffset, int yOffset) override {};

	//��ʾ
	virtual void	Paint(bool bErase = true) override {};

	//˫������
	virtual void	SetDbClickFunc(std::function<bool(void)> func) override {};
};

