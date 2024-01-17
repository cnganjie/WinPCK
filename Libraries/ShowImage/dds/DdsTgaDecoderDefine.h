//////////////////////////////////////////////////////////////////////
// miscpicdlg.h: WinPCK �����̲߳���
// �Ի�����룬����dds��tga��ģ��
//
// �˳����� �����/stsm/liqf ��д
//
// �˴���Ԥ�ƽ��ῪԴ���κλ��ڴ˴�����޸ķ����뱣��ԭ������Ϣ
// 
// 2012.4.10
//////////////////////////////////////////////////////////////////////

#ifndef _MISCPICDLG_H_
#define _MISCPICDLG_H_

//#define	FMT_RAW			0	//bmp,jpg,png,gif,emf.....
//#define	FMT_DDS			1
//#define	FMT_TGA			2

#define	FMTTGA_A1R5G5B5		16
#define	FMTTGA_R8G8B8		24
#define	FMTTGA_A8R8G8B8		32

#define	FMTTGA_RAWTBL	1
#define	FMTTGA_RAWRGB	2
#define	FMTTGA_RLETBL	9
#define	FMTTGA_RLERGB	10

#include <stdint.h>
typedef enum { FMT_DDS, FMT_TGA, FMT_RAW, FMT_UNKNOWN } PICFORMAT;

typedef unsigned __int64 QWORD, *LPQWORD;

#pragma pack(push)
#pragma pack(4)

typedef struct {
	uint8_t	b;
	uint8_t	g;
	uint8_t	r;
	uint8_t	a;
}CBRGB;

typedef union _CMYRGB{
	CBRGB cRGBVal;
	uint32_t dwargb;
}CMYRGB, *LPCRGB;

#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)

typedef struct _TGAHEAD {

	uint8_t		byteTgaInfoSize;		//ͼ����Ϣ�ֶΣ������ӱ�ĺ��棩���ַ���
									//���ֶ��� 1 �ֽ��޷������ͣ�ָ����ͼ���ʽ�����ֶγ���
									//��ȡֵ��Χ�� 0 �� 255 ������Ϊ 0 ʱ��ʾû��ͼ���
									//��Ϣ�ֶΡ�
	uint8_t		byteColorTable;			//��ɫ�������
									//���ֶ�Ϊ��ʾ��Ӧ��ʽ 1 ��ͼ�������һ�������� 1
	uint8_t		byteTgaType;			//ͼ��������
									//���ֶ���Ϊ 1 �� ��Ҳ�Ǵ�����Ϊ��ʽ 1 ��ԭ��
	//��ɫ����,�����ɫ�������ֶ�Ϊ 0 �򱻺��ԣ�������������
	uint16_t	wColorTableOffset;		//��ɫ����ַ
	uint16_t	wColorTableSize;		//��ɫ��ĳ���
	uint8_t		byteColorTableBits;		//��ɫ������λ��bit����
	//ͼ����
	uint16_t	wXOffset;				//ͼ�� x ������ʼλ��
	uint16_t	wYOffset;				//ͼ�� y ������ʼλ��
	uint16_t	wWidth;					//ͼ����
	uint16_t	wHeight;				//ͼ��߶�
	uint8_t		byteBitsPerPixel;		//ͼ��ÿ���ش洢ռ��λ��bit����
	//ͼ���������ֽ�
	uint8_t		byteAttrBitsPerPixel:4;	//bits 3-0 - ÿ���ض�Ӧ������λ��λ����
									//���� TGA 16����ֵΪ 0 �� 1��
									//���� TGA 24����ֵΪ 0��
									//���� TGA 32����ֵΪ 8
	uint8_t		byteTgaOtherConfig:4;	//bit 4    - ����������Ϊ 0
									//bit 5    - ��Ļ��ʼλ�ñ�־
									//0 = ԭ�������½�
									//1 = ԭ�������Ͻ�
									//���� truevision ͼ�����Ϊ 0
									//bits 7-6 - �������ݴ洢��־
									//	00 = �޽���
									//	01 = ��·��/ż����
									//	10 = ��·����
									//	11 = ����
} TGAHEAD, *LPTGAHEAD;

//typedef struct _BMPHEAD {
//
//	UINT16	wBmpHead;
//	UINT32	dwFilesize;
//	UINT32	dwReserved;
//	UINT32	dwBmpDataOffset;
//
//	UINT32	dwBmpHeaderSize;
//	UINT32	dwWidth;
//	UINT32	dwHeight;
//	UINT16	wPlanes;
//	UINT16	wBitsPerPixel;
//	UINT32	dwCompression;
//	UINT32	dwBitmapDataSize;
//	UINT32	dwhresolution;//ec4
//	UINT32	dwvresolution;//ec4
//	UINT32	dwColors;
//	UINT32	dwImportantColors;
//
//	//struct palette{
//	//	BYTE	blue;
//	//	BYTE	green;
//	//	BYTE	red;
//	//	BYTE	zero;
//	//};
//
//} BMPHEAD, *LPBMPHEAD;
//
//typedef struct _BMPHEAD {
//	BITMAPFILEHEADER filehead;
//	BITMAPV4HEADER	DIBhead;
//} BMPHEAD, *LPBMPHEAD;
//

//typedef struct tagBITMAPINFOHEADER{
//  DWORD  biSize; 
//  LONG   biWidth; 
//  LONG   biHeight; 
//  WORD   biPlanes; 
//  WORD   biBitCount; 
//  DWORD  biCompression; 
//  DWORD  biSizeImage; 
//  LONG   biXPelsPerMeter; 
//  LONG   biYPelsPerMeter; 
//  DWORD  biClrUsed; 
//  DWORD  biClrImportant; 
//} BITMAPINFOHEADER, *PBITMAPINFOHEADER; 

//BITMAPINFOHEADER
#pragma pack(pop)

#endif

