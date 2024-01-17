#pragma once

#include "ShowPicture.h"

class CShowPictureWithZoom;

class CShowPictureWithZoom :
	public CShowPicture
{
public:
	//CShowPictureWithZoom(HWND hWndShow, LPBYTE& _buffer, size_t _bufsize);
	//CShowPictureWithZoom(HWND hWndShow, LPBYTE &_buffer, size_t _bufsize, LPCWSTR _lpszFileTitle, PICFORMAT _picFormat);
	CShowPictureWithZoom();
	virtual ~CShowPictureWithZoom();

	virtual bool	Attach(HWND hWndShow, LPBYTE _buffer, size_t _bufsize, LPCWSTR _lpszFileTitle = L"", PICFORMAT _picFormat = FMT_RAW) override;
	virtual void	Detach() override;

#pragma region ����
public:

	//���ŵ����ڴ�С
	virtual void	ZoomToWindowSize() override;

	//�ָ�ԭʼ�ֱ���
	virtual void	ZoomToOriginalResolution(int64_t pos_x, int64_t pos_y) override;

	//ʹ�������ֽ�������
	virtual void	ZoomWithMousePoint(uint32_t nFlags, int16_t zDelta, int64_t pos_x, int64_t pos_y) override;

	//����������
	virtual double	GetZoomRatio() override;
protected:
	//�ڸ������������
	void	ZoomPictureAtPoint(double dZoomRatio, int64_t pos_x, int64_t pos_y);

	//������������m_fixedZoomRatio���µĵ�һ����ֵ
	void	CalcMinZoomRatio();
#pragma endregion

#pragma region ���ڴ�С����
public:
	//���ڴ�С�仯��Ҫ���¼���ֵ
	virtual void	ChangeClientSize(uint16_t nWidth, uint16_t nHeight) override;
	virtual double	CalcFixedRatioByClientSize(uint32_t uRealX, uint32_t uRealY, uint32_t uToX, uint32_t uToY) override;
	//���ڳ�ʼ��ʱ���������ֵ�����ͼƬ���ڷֱ��ʣ����ھ���󻯣�����ֵΪ�����Ƿ���Ҫ��󻯣���Ҫ����TRUE
	virtual bool	CalcFixedRatioAndSizeOnInit(int64_t &_inout_cx, int64_t&_inout_cy, int _in_min_cx, int _in_min_cy) override;

#pragma endregion

#pragma region ��ʾͼ����Ϣ����
public:


	//OldMsgProc = (WNDPROC)SetWindowLong(g_arrHwnd[ID_EDIT_CRC32], GWL_WNDPROC, (LONG)EditProc);
	static LRESULT CALLBACK WinProcStatic(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	//static std::map<HWND, WNDINFO> sm_WinClassMap;
	WNDPROC m_OldMsgProc = nullptr;
#pragma endregion



public:
	//�ƶ�ͼ��
	virtual void	MovePicture(int xOffset, int yOffset) override;

	//��ʾ
	virtual void	Paint(bool bErase = true) override;

	//˫������
private:
	std::function<bool(void)> m_dbClickFunc = nullptr;
public:
	virtual void	SetDbClickFunc(std::function<bool(void)> func) override {
		this->m_dbClickFunc = func;
	};

private:
	BOOL	EvSize(UINT fwSizeType, WORD nWidth, WORD nHeight);
	BOOL	EventButton(UINT uMsg, int nHitTest, POINTS pos);
	BOOL	EvMouseWheel(UINT nFlags, short zDelta, POINTS pos);
	BOOL	EvMouseMove(UINT fwKeys, POINTS pos);
	BOOL	EvPaint(void);

	//��ʾ��ز���
	bool isMouseDown = false;
	//��¼��ǰ���λ��
	POINT			pointMouse{ 0 };

protected:


	//HWND m_hWndShow;

	//double		m_dFixedRatio = 1.0;
	double		m_dZoomRatioShow = 1.0;	//��ǰ������ʾ��������
	double		m_dZoomRatio = 1.0;		//��ǰͼƬ�����ʣ���С��m_fixedZoomRatioʱm_dZoomRatioShow����m_fixedZoomRatio���������m_dZoomRatioShow = m_dZoomRatio
	//��Ӧ��ǰ���ڴ�С ��������
	double		m_fixedZoomRatio = 1.0;

	//��ǰ��ʾ���꣬�Դ������Ͻ�Ϊ0,0
	int64_t		m_ShowX = 0, m_ShowY = 0;
	//��ǰ��ʾ��С
	uint64_t	m_ShowPicWidth = 0;
	uint64_t	m_ShowPicHeight = 0;

};
