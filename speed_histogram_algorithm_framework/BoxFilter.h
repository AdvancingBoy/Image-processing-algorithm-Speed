#pragma once
#include "Core.h"
#include "Utility.h"

// ��������: ʵ��ͼ�񷽿�ģ��Ч��
// �����б�:
// Src: ��Ҫ�����Դͼ������ݽṹ
// Dest: ���洦����ͼ������ݽṹ
// Radius: ����ģ���İ뾶����Ч��Χ[1, 1000]
// EdgeBehavior: ��Ե�����ݵĴ�������0��ʾ�ظ���Ե���أ�1ʹ�þ���ķ�ʽ�Ա�Ե�������ֵ
// ����:
// 1. �ܴ���8λ�ҶȺ�24λͼ��
// 2. Src��Dest������ͬ������ͬʱ�ٶȻ�����

IS_RET BoxBlur(TMatrix *Src, TMatrix *Dest, int Radius, EdgeMode Edge) {
	if (Src == NULL || Dest == NULL) return IS_RET_ERR_NULLREFERENCE;
	if (Src->Data == NULL || Dest->Data == NULL) return IS_RET_ERR_NULLREFERENCE;
	if (Src->Width != Dest->Width || Src->Height != Dest->Height || Src->Channel != Dest->Channel || Src->Depth != Dest->Depth || Src->WidthStep != Dest->WidthStep) return IS_RET_ERR_PARAMISMATCH;
	if (Src->Depth != IS_DEPTH_8U || Dest->Depth != IS_DEPTH_8U) return IS_RET_ERR_NOTSUPPORTED;
	IS_RET Ret = IS_RET_OK;
	TMatrix *Row = NULL, *Col = NULL;
	int *RowPos, *ColPos, *ColSum, *Diff;
	int X, Y, Z, Width, Height, Channel, Index;
	int Value, ValueB, ValueG, ValueR;
	int Size = 2 * Radius + 1, Amount = Size * Size, HalfAmount = Amount / 2;
	Width = Src->Width;
	Height = Src->Height;
	Channel = Src->Channel;
	Ret = GetValidCoordinate(Width, Height, Radius, Radius, Radius, Radius, EdgeMode::Smear, &Row, &Col);		//	��ȡ����ƫ����
	RowPos = ((int *)Row->Data);
	ColPos = ((int *)Col->Data);		   
	ColSum = (int *)IS_AllocMemory(Width * Channel * sizeof(int), true);
	Diff = (int *)IS_AllocMemory((Width - 1) * Channel * sizeof(int), true);
	unsigned char *RowData = (unsigned char *)IS_AllocMemory((Width + 2 * Radius) * Channel, true);
	TMatrix Sum;
	TMatrix *p = &Sum;
	TMatrix **q = &p;
	IS_CreateMatrix(Width, Height, IS_DEPTH_32S, Channel, q);
	for (Y = 0; Y < Height; Y++) {
		unsigned char *LinePS = Src->Data + Y * Src->WidthStep;
		int *LinePD = (int *)(p->Data + Y * p->WidthStep);
		//	����һ�����ݼ���Ե���ֲ��ֵ���ʱ�Ļ�������
		if (Channel == 1)
		{
			for (X = 0; X < Radius; X++)
				RowData[X] = LinePS[RowPos[X]];
			memcpy(RowData + Radius, LinePS, Width);
			for (X = Radius + Width; X < Radius + Width + Radius; X++)
				RowData[X] = LinePS[RowPos[X]];
		}
		else if (Channel == 3)
		{
			for (X = 0; X < Radius; X++)
			{
				Index = RowPos[X] * 3;
				RowData[X * 3] = LinePS[Index];
				RowData[X * 3 + 1] = LinePS[Index + 1];
				RowData[X * 3 + 2] = LinePS[Index + 2];
			}
			memcpy(RowData + Radius * 3, LinePS, Width * 3);
			for (X = Radius + Width; X < Radius + Width + Radius; X++)
			{
				Index = RowPos[X] * 3;
				RowData[X * 3 + 0] = LinePS[Index + 0];
				RowData[X * 3 + 1] = LinePS[Index + 1];
				RowData[X * 3 + 2] = LinePS[Index + 2];
			}
		}
		unsigned char *AddPos = RowData + Size * Channel;
		unsigned char *SubPos = RowData;
		for (X = 0; X < (Width - 1) * Channel; X++)
			Diff[X] = AddPos[X] - SubPos[X];
		//	��һ����Ҫ���⴦��
		if (Channel == 1)
		{
			for (Z = 0, Value = 0; Z < Size; Z++)	Value += RowData[Z];
			LinePD[0] = Value;

			for (X = 1; X < Width; X++)
			{
				Value += Diff[X - 1];	LinePD[X] = Value;				//	����·�����ٶ�������ߺܶ�
			}
		}
		else if (Channel == 3)
		{
			for (Z = 0, ValueB = ValueG = ValueR = 0; Z < Size; Z++)
			{
				ValueB += RowData[Z * 3 + 0];
				ValueG += RowData[Z * 3 + 1];
				ValueR += RowData[Z * 3 + 2];
			}
			LinePD[0] = ValueB;	LinePD[1] = ValueG;	LinePD[2] = ValueR;

			for (X = 1; X < Width; X++)
			{
				Index = X * 3;
				ValueB += Diff[Index - 3];		LinePD[Index + 0] = ValueB;
				ValueG += Diff[Index - 2];		LinePD[Index + 1] = ValueG;
				ValueR += Diff[Index - 1];		LinePD[Index + 2] = ValueR;
			}
		}
	}
	for (Y = 0; Y < Size - 1; Y++)			//	ע��û�����һ��Ŷ						//	����ĺ�ʱֻռ������15%����
	{
		int *LinePS = (int *)(p->Data + ColPos[Y] * p->WidthStep);
		for (X = 0; X < Width * Channel; X++)	ColSum[X] += LinePS[X];
	}

	for (Y = 0; Y < Height; Y++)
	{
		unsigned char* LinePD = Dest->Data + Y * Dest->WidthStep;
		int *AddPos = (int*)(p->Data + ColPos[Y + Size - 1] * p->WidthStep);
		int *SubPos = (int*)(p->Data + ColPos[Y] * p->WidthStep);

		for (X = 0; X < Width * Channel; X++)
		{
			Value = ColSum[X] + AddPos[X];
			LinePD[X] = (Value + HalfAmount) / Amount;					//		+  HalfAmount ��Ҫ��Ϊ����������
			ColSum[X] = Value - SubPos[X];
		}
	}
	IS_FreeMemory(RowPos);
	IS_FreeMemory(ColPos);
	IS_FreeMemory(Diff);
	IS_FreeMemory(ColSum);
	IS_FreeMemory(RowData);
	return Ret;
}

