//////////////////////////////////////////////////////////////////////
// DrawPicture.cpp: ��ʾdds��tgaͼ��
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
#include "dds/DdsTgaDecoder.h"

BOOL CShowPicture::BuildImage()
{

	try {

		switch (this->m_picFormat) {

#if INIT_DDS_MOD
		case FMT_DDS:
		{
			CDdsTgaDecoder cDdsTgaDecoder;

			if (!cDdsTgaDecoder.DecodeDDS(m_resBufferInClass, m_bufsize, m_DecodedDIBData, m_picWidth, m_picHeight, m_szPictureFormat))
				throw std::runtime_error("gui-str.show-picture-error");
			{
				free(m_resBufferInClass);
				m_resBufferInClass = nullptr;
				m_lpmyImage = new Gdiplus::Bitmap(m_picWidth, m_picHeight, cDdsTgaDecoder.GetStride(), cDdsTgaDecoder.GetPixelFormat(), m_DecodedDIBData);
			}
		}
			break;

		case FMT_TGA:
		{
			CDdsTgaDecoder cDdsTgaDecoder;
			if (!cDdsTgaDecoder.DecodeTGA(m_resBufferInClass, m_bufsize, m_DecodedDIBData, m_picWidth, m_picHeight, m_szPictureFormat))
				throw std::runtime_error("gui-str.show-picture-error");

			free(m_resBufferInClass);
			m_resBufferInClass = nullptr;

			m_lpmyImage = new Gdiplus::Bitmap(m_picWidth, m_picHeight, cDdsTgaDecoder.GetStride(), cDdsTgaDecoder.GetPixelFormat(), m_DecodedDIBData);
		}
			break;
#endif
		case FMT_RAW:	//bmp,jpg,png..

			CreateStreamOnHGlobal(m_hGlobal, FALSE, &m_stream);
			if (!m_stream)
				throw std::runtime_error("gui-str.show-picture-error");

			//show picture
			m_lpmyImage = new Gdiplus::Image(m_stream);

			//����ͼƬ��С����ʾ���ڴ�С
			m_picWidth = m_lpmyImage->GetWidth();
			m_picHeight = m_lpmyImage->GetHeight();

			break;

		default:
			throw std::runtime_error("gui-str.show-picture-unknown-type");
		}
	}
	catch (std::exception ex)
	{
		MessageBoxA(NULL, ex.what(), "gui-str.error", MB_OK | MB_ICONERROR);
		assert(FALSE);
		return FALSE;
	}

	return TRUE;
}

BOOL CShowPicture::DrawPictureOnDC()
{
	DrawBlockOnDlg();

	if(nullptr != m_MemDC){
		//���ڽ�m_lpmyImage�е�ͼ�񻭵�m_MemDC��
		std::unique_ptr<Gdiplus::Graphics> pGraph(new Gdiplus::Graphics(m_MemDC));
		if (nullptr != pGraph) {
			pGraph->DrawImage(m_lpmyImage, 0, 0, m_picWidth, m_picHeight);
		}
	}

	//������Դ�������ǵ�FMT_RAW == iFormatʱִ�е�
	if (FMT_RAW == m_picFormat) {

		//�����������ͷ�m_lpmyImage������Ϊpngʱ���õ�
		if(NULL != m_lpmyImage)
			delete m_lpmyImage;
		m_lpmyImage = NULL;

		if (NULL != m_stream)
			m_stream->Release();
		m_stream = NULL;

		if (NULL != m_hGlobal)
			GlobalFree(m_hGlobal);
		m_hGlobal = NULL;
	}

	return TRUE;
}

//�ڴ���������ʾ��͸�������ĸ���
BOOL CShowPicture::DrawBlockOnDlg()
{
	HDC pDC = ::GetDC(m_hWndShow);

	//���������Ļ��ʾ���ݵ��ڴ���ʾ�豸  
	m_MemDC = CreateCompatibleDC(NULL);
	//��ʾ�豸����
	HDC MemDCTemp = CreateCompatibleDC(NULL);
	//��ʱ�����ܻ�ͼ����Ϊû�еط���
	//���潨��һ������Ļ��ʾ���ݵ�λͼ��λͼ�Ĵ�С�����ô��ڵĴ�С 
	m_MemBitmap = CreateCompatibleBitmap(pDC, m_picWidth, m_picHeight);
	//λͼ����
	HBITMAP MemBitmapTemp = CreateCompatibleBitmap(pDC, 24, 8);

	ReleaseDC(m_hWndShow, pDC);

	//��λͼѡ�뵽�ڴ���ʾ�豸�� 
	//ֻ��ѡ����λͼ���ڴ���ʾ�豸���еط���ͼ������ָ����λͼ�� 
	HBITMAP pOldBit = (HBITMAP)SelectObject(m_MemDC, m_MemBitmap);
	HBITMAP pOldBit1 = (HBITMAP)SelectObject(MemDCTemp, MemBitmapTemp);

	//���ñ���ɫ��λͼ����ɾ����������õ��ǰ�ɫ��Ϊ���� 
	//��Ҳ�������Լ�Ӧ���õ���ɫ 
	SetBkColor(MemDCTemp, RGB(204, 204, 204));
	RECT thisrect = { 0, 0, 24, 8 };
	ExtTextOut(MemDCTemp, 0, 0, ETO_OPAQUE, &thisrect, NULL, 0, NULL);

	thisrect.left += 8;
	SetBkColor(MemDCTemp, RGB(255, 255, 255));
	ExtTextOut(MemDCTemp, 0, 0, ETO_OPAQUE, &thisrect, NULL, 0, NULL);

	thisrect.left += 8;
	SetBkColor(MemDCTemp, RGB(204, 204, 204));
	ExtTextOut(MemDCTemp, 0, 0, ETO_OPAQUE, &thisrect, NULL, 0, NULL);

	for (int j = 0; j < m_picHeight; j += 8) {
		for (int i = 0; i < m_picWidth; i += 16) {
			if (j & 8)
				BitBlt(m_MemDC, i, j, i + 16, j + 8, MemDCTemp, 0, 0, SRCCOPY);
			else
				BitBlt(m_MemDC, i, j, i + 16, j + 8, MemDCTemp, 8, 0, SRCCOPY);
		}
	}
	//MemBitmap.DeleteObject(); 
	SelectObject(MemDCTemp, pOldBit1);
	//a = DeleteObject(MemBitmap);
	DeleteObject(MemBitmapTemp);
	//MemDC.DeleteDC();
	DeleteDC(MemDCTemp);

	return TRUE;
}
