//////////////////////////////////////////////////////////////////////
// ShowPictureWithZoom.cpp: ��ʾ���ŵ�dds��tgaͼ��
// �������dds��tgaͼ��������ź������ʾ
//
// �˳����� �����/stsm/liqf ��д
//
// �˴���Ԥ�ƽ��ῪԴ���κλ��ڴ˴�����޸ķ����뱣��ԭ������Ϣ
// 
// 2018.5.29
//////////////////////////////////////////////////////////////////////
#include "showimgpch.h"
#include "ShowPictureWithZoom.h"

#pragma warning ( disable : 4018 )
#pragma warning ( disable : 4244 )
#pragma warning ( disable : 4838 )

#define CHANGE_RATIO	1.2
#define MAX_ZOOM_RATIO	32.0

CShowPictureWithZoom::CShowPictureWithZoom() : CShowPicture()
{
}

CShowPictureWithZoom::~CShowPictureWithZoom()
{
	Detach();
}

#pragma region ����

//���ŵ����ڴ�С
void CShowPictureWithZoom::ZoomToWindowSize()
{
	RECT rect;
	GetClientRect(m_hWndShow, &rect);

	//����һ�¶�������õ�������
	m_fixedZoomRatio = m_dZoomRatioShow = CalcFixedRatioByClientSize(m_picWidth, m_picHeight, rect.right, rect.bottom);

	m_ShowPicWidth = m_picWidth * m_dZoomRatioShow;
	m_ShowPicHeight = m_picHeight * m_dZoomRatioShow;

	m_ShowX = (static_cast<int>(rect.right) - static_cast<int>(m_ShowPicWidth)) / 2;
	m_ShowY = (static_cast<int>(rect.bottom) - static_cast<int>(m_ShowPicHeight)) / 2;

	Paint();
}

void CShowPictureWithZoom::ZoomToOriginalResolution(int64_t pos_x, int64_t pos_y)
{
	m_dZoomRatioShow = m_dZoomRatio = 1.0;
	ZoomPictureAtPoint(1.0, pos_x, pos_y);
}

void CShowPictureWithZoom::ZoomWithMousePoint(uint32_t nFlags, int16_t zDelta, int64_t pos_x, int64_t pos_y)
{

	//���Ŵ���Ϊ8������СΪ��Ӧ��ǰ���ڴ�С��1.0
	double dZoomChangeRatio = 1.0;
	double dNextZoomRatio = 1.0;
	//�Ŵ�
	if (0 < zDelta) {

		dZoomChangeRatio *= (zDelta / 120.0 * CHANGE_RATIO);

		if ((MAX_ZOOM_RATIO + EPSILON) < m_dZoomRatio)
			return;

		dNextZoomRatio = m_dZoomRatio * dZoomChangeRatio;

		if (dNextZoomRatio > MAX_ZOOM_RATIO) {

			m_dZoomRatioShow = MAX_ZOOM_RATIO;
		}
		else {

			m_dZoomRatioShow = dNextZoomRatio;
		}

		//��С
	}
	else if (0 > zDelta) {
		dZoomChangeRatio /= (zDelta / (-120.0) * CHANGE_RATIO);

		//�������ŷ�Χ
		double dMinZoomRatio = (m_fixedZoomRatio > (1.0 - EPSILON)) ? 1.0 : m_fixedZoomRatio;

		if (dMinZoomRatio > (m_dZoomRatio + EPSILON))
			return;

		dNextZoomRatio = m_dZoomRatio * dZoomChangeRatio;

		if (dNextZoomRatio < dMinZoomRatio) {

			m_dZoomRatioShow = dMinZoomRatio;
		}
		else {

			m_dZoomRatioShow = dNextZoomRatio;
		}

	}

	m_dZoomRatio = dNextZoomRatio;

	ZoomPictureAtPoint(m_dZoomRatioShow, pos_x, pos_y);
}

double CShowPictureWithZoom::GetZoomRatio()
{
	return m_dZoomRatioShow;
}

static void CalcZoomPictureAtOneDimensional(int iClientPointXY, int64_t& _showXY, uint64_t& _picWidthHeight, double _dZoomRatio, uint32_t _RealPicWidthHeight, LONG	lClientWidthHeight)
{
	/*
	����m_ShowX,m_ShowY
	��ǰ��ʾ��С���ڴ���ʱ��
	�����Ե�ǰ���λ��Ϊ��������
	�������ĵ���ͼƬ�ϵ�λ��Ϊrx,ry, ����ڴ����ϵ�λ��Ϊmx,my��ͼƬ0,0�ڴ����ϵ���ʾ��Ϊsx,sy����rx=mx-sx
	�����Ե�ǰ���λ��Ϊ�������ţ��������ͼƬ�ϵĵ㲻������������ֵ��������x�����ϵ�����ֵΪp,�赱ǰͼƬ���Ϊsw����p=rx/sw=(mx-sx)/sw
	���ź�ͼƬ��ȱ仯Ϊsw1,��ʾ��仯Ϊsx1��p���䣬p=(mx-sx1)/sw1���õ�sx1 = mx - (mx-sx)*sw1/sw
	*/

	int mouseXY = iClientPointXY;
	//��������ͼ��֮��ʱ����ͼ��߽�Ϊ׼
	if (_showXY > mouseXY)mouseXY = _showXY;
	int64_t iShowRightBottom = _showXY + _picWidthHeight;
	if (iShowRightBottom < mouseXY)mouseXY = iShowRightBottom;

	//�������ź��m_ShowX, m_ShowY
	uint64_t	dwShowPicWidthHeight = _dZoomRatio * _RealPicWidthHeight;

	//����ʾ��ͼƬ��ȴ��ڵ�ǰ����ʱ

	if (lClientWidthHeight < dwShowPicWidthHeight) {
		_showXY = mouseXY - (mouseXY - _showXY) * dwShowPicWidthHeight / _picWidthHeight;
		if (0 < _showXY)_showXY = 0;
	}
	else {

		_showXY = (lClientWidthHeight - dwShowPicWidthHeight) / 2;
	}

	_picWidthHeight = dwShowPicWidthHeight;
}

void CShowPictureWithZoom::ZoomPictureAtPoint(double dZoomRatio, int64_t pos_x, int64_t pos_y)
{

	//�ֱ����X��Y�������Ե�ǰ��������ŵĽ��

	RECT	rectWin;
	GetClientRect(m_hWndShow, &rectWin);

	CalcZoomPictureAtOneDimensional(pos_x, m_ShowX, m_ShowPicWidth, dZoomRatio, m_picWidth, rectWin.right);
	CalcZoomPictureAtOneDimensional(pos_y, m_ShowY, m_ShowPicHeight, dZoomRatio, m_picHeight, rectWin.bottom);

	Paint();
}

void CShowPictureWithZoom::CalcMinZoomRatio()
{
	double dZoomRatio;

	for (dZoomRatio = 1.0; dZoomRatio >= m_dZoomRatioShow; dZoomRatio /= CHANGE_RATIO) {
	}

	m_dZoomRatio = dZoomRatio;
}

#pragma endregion
#pragma region ���ڴ�С����

void CShowPictureWithZoom::ChangeClientSize(uint16_t nWidth, uint16_t nHeight)
{
	this->m_ShowX = (static_cast<int>(nWidth) - static_cast<int>(this->m_ShowPicWidth)) / 2;
	this->m_ShowY = (static_cast<int>(nHeight) - static_cast<int>(this->m_ShowPicHeight)) / 2;

	//����һ�¶�������õ�������
	this->m_fixedZoomRatio = CalcFixedRatioByClientSize(this->m_picWidth, this->m_picHeight, nWidth, nHeight);

	Paint();
}

double CShowPictureWithZoom::CalcFixedRatioByClientSize(uint32_t uRealX, uint32_t uRealY, uint32_t uToX, uint32_t uToY)
{
	if ((0 == uRealX) || (0 == uRealY))
		return 1.0;

	//����һ�¶�������õ�������
	double zoomx = static_cast<double>(uToX) / static_cast<double>(uRealX);
	double zoomy = static_cast<double>(uToY) / static_cast<double>(uRealY);

	return zoomx > zoomy ? zoomy : zoomx;
}

bool CShowPictureWithZoom::CalcFixedRatioAndSizeOnInit(int64_t& _inout_cx, int64_t& _inout_cy, int _in_min_cx, int _in_min_cy)
{

	//��ǰ�ֱ���
	const uint32_t width = GetSystemMetrics(SM_CXSCREEN);
	const uint32_t height = GetSystemMetrics(SM_CYSCREEN);

	double screenx_d_realx = static_cast<double>(width) / static_cast<double>(m_picWidth);
	double screeny_d_realy = static_cast<double>(height) / static_cast<double>(m_picHeight);

	bool bNeedShowMax = true;
	//�Ƚ�screenx_d_realx��screeny_d_realy���ĸ�С˵���ĸ�����Խ��
	if (screenx_d_realx < screeny_d_realy) {

		if (width < m_picWidth) {
			m_dZoomRatio = screenx_d_realx;

		}
		else {
			bNeedShowMax = false;

		}
	}
	else {

		if (height < m_picHeight) {
			m_dZoomRatio = screeny_d_realy;

		}
		else {
			bNeedShowMax = false;

		}
	}
	m_dZoomRatioShow = m_dZoomRatio;

	if ((1.0 - EPSILON) > m_dZoomRatioShow) {
		CalcMinZoomRatio();
	}


	m_ShowPicWidth = m_dZoomRatioShow * m_picWidth;
	m_ShowPicHeight = m_dZoomRatioShow * m_picHeight;

	if (bNeedShowMax) {
		return true;
	}
	else {
		_inout_cx = (m_picWidth > _in_min_cx) ? m_picWidth + 10 : _in_min_cx + 10;
		_inout_cy = (m_picHeight > _in_min_cy) ? m_picHeight + 30 : _in_min_cy + 30;
		return false;
	}

}

#pragma endregion


void CShowPictureWithZoom::MovePicture(int xOffset, int yOffset)
{

	RECT	rectWin;
	GetClientRect(m_hWndShow, &rectWin);

	//����ʾ��ͼƬ��ȴ��ڵ�ǰ����ʱ
	if(rectWin.right < m_ShowPicWidth) {

		int min_x = static_cast<int>(rectWin.right) - static_cast<int>(m_ShowPicWidth);

		//����ʾ������ڴ���ʱ��ͼƬ���Ͻǲ��ܴ���0
		//�������
		if(xOffset > 0) {

			if(min_x <= m_ShowX) {
				m_ShowX -= xOffset;
				if(m_ShowX < min_x) m_ShowX = min_x;
			}
		} else {

			if(0 >= m_ShowX) {
				m_ShowX -= xOffset;
				if(m_ShowX > 0)m_ShowX = 0;
			}
		}

	}

	//����ʾ��ͼƬ�߶ȴ��ڵ�ǰ����ʱ
	if(rectWin.bottom < m_ShowPicHeight) {

		int min_y = static_cast<int>(rectWin.bottom) - static_cast<int>(m_ShowPicHeight);

		//������϶�
		if(yOffset > 0) {

			if(min_y <= m_ShowY) {
				m_ShowY -= yOffset;
				if(m_ShowY < min_y) m_ShowY = min_y;
			}
		} else {

			if(0 >= m_ShowY) {
				m_ShowY -= yOffset;
				if(m_ShowY > 0)m_ShowY = 0;
			}
		}

	}

	Paint();
}

void CShowPictureWithZoom::Paint(bool bErase)
{
	CShowPicture::Paint(m_ShowX, m_ShowY, 0, 0, m_dZoomRatioShow, bErase);
}