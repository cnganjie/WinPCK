#pragma once
#include "DdsTgaDecoderDefine.h"
#include "dxgiformat.h"


#if INIT_DDS_MOD

class CDdsTgaDecoder
{
public:
	CDdsTgaDecoder();
	~CDdsTgaDecoder();


	BOOL	Decode(LPBYTE _In_data, UINT32 _in_bufsize, LPBYTE &_out_data, UINT &_out__picWidth, UINT &_out_picHeight, PICFORMAT picFormat, LPSTR _out_szFormatDdsTga);
	BOOL	DecodeDDS(LPBYTE _In_data, UINT32 _in_bufsize, LPBYTE &_out_data, UINT &_out__picWidth, UINT &_out_picHeight, LPSTR _out_szFormatDdsTga);
	BOOL	DecodeTGA(LPBYTE _In_data, UINT32 _in_bufsize, LPBYTE &_out_data, UINT &_out__picWidth, UINT &_out_picHeight, LPSTR _out_szFormatDdsTga);

	//����stride��ֵ
	UINT	GetStride();
	//����ͼ��Ŀ�͸�
	UINT	GetWidth();
	UINT	GetHeight();
	//����ͼ���m_decodedDIBFormat
	PixelFormat GetPixelFormat();

protected:

	//ͼƬ�Ŀ�͸�
	UINT		m_picWidth, m_picHeight;
	//ͼƬ����dds,tga,other
	//enum { FMT_DDS, FMT_TGA, FMT_RAW} m_picFormat;
	//PICFORMAT	m_picFormat;
	//��ʾ��ͼƬ�ľ����ʽ
	LPSTR		m_lpszPictureFormat;
	//���ڲ�ʹ�õ�ԭʼ��ͼƬ�ļ����ݣ�������ΪRAWʱ��ָ��hGlobal���ڴ�
	LPBYTE		m_resBufferInClass;
	//������ļ������ݵĴ�С
	UINT32		m_bufsize;
	//�Ѿ������ͼƬ��DIB���ݣ�DDS��TGA��
	char*		m_DecodedDIBData;
	//�Ѿ������ͼƬ��DIB���ݣ�DDS��TGA���Ĵ�С
	DWORD		m_dwDdsTgaToDIBDataSize;
	//
	PixelFormat	m_decodedDIBFormat;
	//һ������һ�����ٸ��ֽ�
	UINT		m_bytesPerLine;
	//һ������һ�����ٸ����ص�
	UINT		m_bytesPerPixel;
	//ͼƬһ�����ص��������ڴ�����Ĵ�С(align=4byte)
	UINT		m_stride;

	//��ȡ��dds,tga���ڲ�DIB�ĸ�ʽ��˳�����һ��stride(ͼƬһ�����ص��������ڴ�����Ĵ�С),...��dds��ʽʱ����D3DFORMAT��tga��ʽʱ����tga��ʽ��int ,FMTTGA_A1R5G5B5...)
	PixelFormat DetectDdsFormatAndAllocByFormat(D3DFORMAT fmt);
	PixelFormat DetectTgaFormatAndAllocByFormat(int fmt);
	BOOL CalaStride_DetectDIBFormat_AllocByFormat(UINT16 wWidth, UINT16 wHeight, PixelFormat pixelFormat);
	BOOL GetDDSFormatStringAndDecodeEntry(D3DFORMAT fmt, LPSTR lpszPictureFormat, void(CDdsTgaDecoder::* &decode_dds_dxtN)(LPBYTE));
	//��dds,tga�����ݴ���DIB������
	BOOL CreateBmpBufferByDds();
	BOOL CreateBmpBufferByTga();

#pragma region decode_dds_����
	void	decode_dds_dxt1(BYTE *ddsimage);
	void	decode_dds_dxt3(BYTE *ddsimage);
	void	decode_dds_dxt5(BYTE *ddsimage);
	void	decode_dds_a8r8g8b8(BYTE *ddsimage);
	void	decode_dds_x8r8g8b8(BYTE *ddsimage);
	void	decode_dds_a8b8g8r8(BYTE *ddsimage);
	void	decode_dds_x8b8g8r8(BYTE *ddsimage);
	void	decode_dds_r8g8b8(BYTE *ddsimage);
	void	decode_dds_a1r5g5b5(BYTE *ddsimage);
	void	decode_dds_x1r5g5b5(BYTE *ddsimage);
	void	decode_dds_r5g6b5(BYTE *ddsimage);
	void	decode_dds_a4r4g4b4(BYTE *ddsimage);
	void	decode_dds_x4r4g4b4(BYTE *ddsimage);
	void	decode_dds_a8r3g3b2(BYTE *ddsimage);
	void	decode_dds_a2r10g10b10(BYTE *ddsimage);
	void	decode_dds_a2b10g10r10(BYTE *ddsimage);
#pragma endregion

#pragma region TGA decoder
	BOOL	makeTgaColorMappedData(BYTE * &tgaimage, int fmt, char * &bufferOfColorMappedData, UINT16 wColorTableSize/*, int nTgaBitsPerPixel*/);
	void	decode_tga_ColorMapped8(BYTE *tgaimage, char *lpColorMappedData);
	void	decode_tga_ColorMapped16(BYTE *tgaimage, char *lpColorMappedData);

	void	decode_tga_RGB(BYTE *tgaimage);

	void	decode_tga_ColorMapped8REL(BYTE *tgaimage, char *lpColorMappedData);
	void	decode_tga_ColorMapped16REL(BYTE *tgaimage, char *lpColorMappedData);

	void	decode_tga_RGBREL(BYTE *tgaimage);
#pragma endregion
};

#endif
