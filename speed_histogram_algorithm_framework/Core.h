#pragma once
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <opencv2/opencv.hpp>
using namespace std;

#define WIDTHBYTES(bytes) (((bytes * 8) + 31) / 32 * 4)
const float Inv255 = 1.0 / 255;
const double Eps = 2.220446049250313E-16;


//��Ե���ķ�ʽ
enum EdgeMode {
	Tile = 0, //�ظ���ԵԪ��
	Smear = 1 //�����ԵԪ��
};

enum IS_RET {
	IS_RET_OK,									//	����
	IS_RET_ERR_OUTOFMEMORY,						//	�ڴ����
	IS_RET_ERR_STACKOVERFLOW,					//	��ջ���
	IS_RET_ERR_NULLREFERENCE,					//	������
	IS_RET_ERR_ARGUMENTOUTOFRANGE,				//	��������������Χ��
	IS_RET_ERR_PARAMISMATCH,					//	������ƥ��
	IS_RET_ERR_DIVIDEBYZERO,
	IS_RET_ERR_INDEXOUTOFRANGE,
	IS_RET_ERR_NOTSUPPORTED,
	IS_RET_ERR_OVERFLOW,
	IS_RET_ERR_FILENOTFOUND,
	IS_RET_ERR_UNKNOWN
};

enum IS_DEPTH
{
	IS_DEPTH_8U = 0,			//	unsigned char
	IS_DEPTH_8S = 1,			//	char
	IS_DEPTH_16S = 2,			//	short
	IS_DEPTH_32S = 3,			//  int
	IS_DEPTH_32F = 4,			//	float
	IS_DEPTH_64F = 5,			//	double
};

struct TMatrix
{
	int Width;					//	����Ŀ��
	int Height;					//	����ĸ߶�
	int WidthStep;				//	����һ��Ԫ�ص�ռ�õ��ֽ���
	int Channel;				//	����ͨ����
	int Depth;					//	����Ԫ�ص�����
	unsigned char *Data;		//	���������
	int Reserved;				//	����ʹ��
};

// �ڴ�����
void *IS_AllocMemory(unsigned int Size, bool ZeroMemory = true) {
	void *Ptr = _mm_malloc(Size, 32);
	if (Ptr != NULL)
		if (ZeroMemory == true)
			memset(Ptr, 0, Size);
	return Ptr;
}

// �ڴ��ͷ�
void IS_FreeMemory(void *Ptr) {
	if (Ptr != NULL) _mm_free(Ptr);
}

// ���ݾ���Ԫ�ص���������ȡһ��Ԫ��ʵ��ռ�õ��ֽ���
int IS_ELEMENT_SIZE(int Depth) {
	int Size;
	switch (Depth)
	{
	case IS_DEPTH_8U:
		Size = sizeof(unsigned char);
		break;
	case IS_DEPTH_8S:
		Size = sizeof(char);
		break;
	case IS_DEPTH_16S:
		Size = sizeof(short);
		break;
	case IS_DEPTH_32S:
		Size = sizeof(int);
		break;
	case IS_DEPTH_32F:
		Size = sizeof(float);
		break;
	case IS_DEPTH_64F:
		Size = sizeof(double);
		break;
	default:
		Size = 0;
		break;
	}
	return Size;
}

//�����µľ�������
IS_RET IS_CreateMatrix(int Width, int Height, int Depth, int Channel, TMatrix **Matrix) {
	if (Width < 1 || Height < 1) return IS_RET_ERR_ARGUMENTOUTOFRANGE; //��������������Χ��
	if (Depth != IS_DEPTH_8U && Depth != IS_DEPTH_8S && Depth != IS_DEPTH_16S && Depth != IS_DEPTH_32S &&
		Depth != IS_DEPTH_32F && Depth != IS_DEPTH_64F) return IS_RET_ERR_ARGUMENTOUTOFRANGE; //��������������Χ��
	if (Channel != 1 && Channel != 2 && Channel != 3 && Channel != 4) return IS_RET_ERR_ARGUMENTOUTOFRANGE;
	*Matrix = (TMatrix *)IS_AllocMemory(sizeof(TMatrix));
	(*Matrix)->Width = Width;
	(*Matrix)->Height = Height;
	(*Matrix)->Depth = Depth;
	(*Matrix)->Channel = Channel;
	(*Matrix)->WidthStep = WIDTHBYTES(Width * Channel * IS_ELEMENT_SIZE(Depth));
	(*Matrix)->Data = (unsigned char*)IS_AllocMemory((*Matrix)->Height * (*Matrix)->WidthStep, true);
	if ((*Matrix)->Data == NULL) {
		IS_FreeMemory(*Matrix);
		return IS_RET_ERR_OUTOFMEMORY; //�ڴ����
	}
	(*Matrix)->Reserved = 0;
	return IS_RET_OK;
}

//�ͷŴ����ľ���
IS_RET IS_FreeMatrix(TMatrix **Matrix) {
	if ((*Matrix) == NULL) return IS_RET_ERR_NULLREFERENCE; //������
	if ((*Matrix)->Data == NULL) {
		IS_FreeMemory((*Matrix));
		return IS_RET_ERR_OUTOFMEMORY;
	}
	else {
		IS_FreeMemory((*Matrix)->Data);
		IS_FreeMemory((*Matrix));
		return IS_RET_OK;
	}
}

//��¡���еľ���
IS_RET IS_CloneMatrix(TMatrix *Src, TMatrix **Dest) {
	if (Src == NULL) return IS_RET_ERR_NULLREFERENCE;
	if (Src->Data == NULL) return IS_RET_ERR_NULLREFERENCE;
	IS_RET Ret = IS_CreateMatrix(Src->Width, Src->Height, Src->Depth, Src->Channel, Dest);
	if (Ret == IS_RET_OK) memcpy((*Dest)->Data, Src->Data, (*Dest)->Height * (*Dest)->WidthStep);
	return Ret;
}