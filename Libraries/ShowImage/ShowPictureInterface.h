#pragma once

#include "dds/DdsTgaDecoderDefine.h"
#pragma comment(lib,"gdiplus.lib")
#include <functional>

class IShowPicture
{
public:
	//IShowPicture() {};
	virtual ~IShowPicture() = default;

	virtual bool	Attach(HWND hWndShow, uint8_t* _buffer, size_t _bufsize, const wchar_t* _lpszFileTitle = L"", PICFORMAT _picFormat = FMT_RAW) = 0;
	virtual void	Detach() = 0;

	//����ͼ��Ŀ�͸�
	virtual uint32_t	GetWidth() const = 0;
	virtual uint32_t	GetHeight() const = 0;

	//
	virtual bool	IsDecoded() const = 0;

	virtual std::wstring	GetWindowTitle() const = 0;

	//���豸����ʾͼ��
	virtual bool	Paint(int64_t nXOriginDest, int64_t nYOriginDest, int nXOriginSrc, int nYOriginSrc, double dZoomRatio = 1.00, bool bErase = true) = 0;
	//������ʾ��ͼƬ
	virtual bool	Save(const wchar_t* lpszFilename) = 0;

	//���ŵ����ڴ�С
	virtual void	ZoomToWindowSize() = 0;

	//�ָ�ԭʼ�ֱ���
	virtual void	ZoomToOriginalResolution(int64_t pos_x, int64_t pos_y) = 0;

	//ʹ�������ֽ�������
	virtual void	ZoomWithMousePoint(uint32_t nFlags, int16_t zDelta, int64_t pos_x, int64_t pos_y) = 0;

	//����������
	virtual double	GetZoomRatio() = 0;

	//���ڴ�С�仯��Ҫ���¼���ֵ
	virtual void	ChangeClientSize(uint16_t nWidth, uint16_t nHeight) = 0;
	virtual double	CalcFixedRatioByClientSize(uint32_t uRealX, uint32_t uRealY, uint32_t uToX, uint32_t uToY) = 0;
	//���ڳ�ʼ��ʱ���������ֵ�����ͼƬ���ڷֱ��ʣ����ھ���󻯣�����ֵΪ�����Ƿ���Ҫ��󻯣���Ҫ����TRUE
	virtual bool	CalcFixedRatioAndSizeOnInit(int64_t& _inout_cx, int64_t& _inout_cy, int _in_min_cx, int _in_min_cy) = 0;


	//�ƶ�ͼ��
	virtual void	MovePicture(int xOffset, int yOffset) = 0;

	//��ʾ
	virtual void	Paint(bool bErase = true) = 0;

	//˫������
	virtual void	SetDbClickFunc(std::function<bool(void)> func) = 0;
};

std::unique_ptr<IShowPicture> MakeShowPictureInstance();
std::unique_ptr<IShowPicture> MakeZoomShowPictureInstance();
