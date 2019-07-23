#pragma once
//����ֵ
#include "Core.h"

union Approximation
{
	double Value;
	int X[2];
};

// ����1: �����ݽض���Byte���������ڡ�
// �ο�: http://www.cnblogs.com/zyl910/archive/2012/03/12/noifopex1.html
// ���: ��λ���������ʹ����ô����������������롣
unsigned char ClampToByte(int Value) {
	return ((Value | ((signed int)(255 - Value) >> 31)) & ~((signed int)Value >> 31));
}

//����2: �����ݽض���ָ����Χ��
//�ο�: ��
//���: ��
int ClampToInt(int Value, int Min, int Max) {
	if (Value < Min) return Min;
	else if (Value > Max) return Max;
	else return Value;
}

//����3: ��������255
//�ο�: ��
//���: ��λ
int Div255(int Value) {
	return (((Value >> 8) + Value + 1) >> 8);
}

//����4: ȡ����ֵ
//�ο�: https://oi-wiki.org/math/bit/
//���: ��n > 0 ? n : -n ��

int Abs(int n) {
	return (n ^ (n >> 31)) - (n >> 31);
	/* n>>31 ȡ�� n �ķ��ţ��� n Ϊ������n>>31 ���� 0���� n Ϊ������n>>31 ���� - 1
	�� n Ϊ���� n^0=0, �����䣬�� n Ϊ������ n^-1
	��Ҫ���� n �� - 1 �Ĳ��룬Ȼ�����������㣬
	��� n ��Ų���Ϊ n �ľ���ֵ�� 1���ټ�ȥ - 1 ���Ǿ���ֵ */
}

//����5: ��������
//�ο�: ��
//���: ��
double Round(double V)
{
	return (V > 0.0) ? floor(V + 0.5) : Round(V - 0.5);
}

//����6: ����-1��1֮��������
//�ο�: ��
//���: ��
double Rand()
{
	return (double)rand() / (RAND_MAX + 1.0);
}

//����7: Pow�����Ľ��Ƽ��㣬���double���ͺ�float����
//�ο�: http://www.cvchina.info/2010/03/19/log-pow-exp-approximation/
//�ο�: http://martin.ankerl.com/2007/10/04/optimized-pow-approximation-for-java-and-c-c/
//���: �������ֻ��Ϊ�˼��ٵĽ��Ƽ��㣬��5%-12%���ȵ����
double Pow(double X, double Y)
{
	Approximation V = { X };
	V.X[1] = (int)(Y * (V.X[1] - 1072632447) + 1072632447);
	V.X[0] = 0;
	return V.Value;
}


float Pow(float X, float Y)
{
	Approximation V = { X };
	V.X[1] = (int)(Y * (V.X[1] - 1072632447) + 1072632447);
	V.X[0] = 0;
	return (float)V.Value;
}

//����8: Exp�����Ľ��Ƽ��㣬���double���ͺ�float����
double Exp(double Y)			//	��������ķ�ʽ���ٶ�Ҫ��Щ
{
	Approximation V;
	V.X[1] = (int)(Y * 1485963 + 1072632447);
	V.X[0] = 0;
	return V.Value;
}

float Exp(float Y)			//	��������ķ�ʽ���ٶ�Ҫ��Щ
{
	Approximation V;
	V.X[1] = (int)(Y * 1485963 + 1072632447);
	V.X[0] = 0;
	return (float)V.Value;
}

// ����9: Pow������׼һ��Ľ��Ƽ��㣬�����ٶȻ�����
// http://martin.ankerl.com/2012/01/25/optimized-approximative-pow-in-c-and-cpp/
// Besides that, I also have now a slower approximation that has much less error
// when the exponent is larger than 1. It makes use exponentiation by squaring,
// which is exact for the integer part of the exponent, and uses only the exponent��s fraction for the approximation:
// should be much more precise with large Y

double PrecisePow(double X, double Y) {
	// calculate approximation with fraction of the exponent
	int e = (int)Y;
	Approximation V = { X };
	V.X[1] = (int)((Y - e) * (V.X[1] - 1072632447) + 1072632447);
	V.X[0] = 0;
	// exponentiation by squaring with the exponent's integer part
	// double r = u.d makes everything much slower, not sure why
	double r = 1.0;
	while (e)
	{
		if (e & 1)	r *= X;
		X *= X;
		e >>= 1;
	}
	return r * V.Value;
}

//����10: ����Min��Max֮��������
//�ο�: ��
//���: MinΪ���������Сֵ��MaxΪ����������ֵ
int Random(int Min, int Max) {
	return rand() % (Max + 1 - Min) + Min;
}

//����11: ���ź���
//�ο�: ��
//���: ��
int sgn(int X) {
	if (X > 0) return 1;
	if (X < 0) return -1;
	return 0;
}

//����12: ��ȡĳ�����α�����Ӧ����ɫֵ
//�ο�: ��
//���: ��
void GetRGB(int Color, int *R, int *G, int *B) {
	*R = Color & 255;
	*G = (Color & 65280) / 256;
	*B = (Color & 16711680) / 65536;
}

//����13: ţ�ٷ����ƻ�ȡָ�����ֵ��㷨ƽ����
//�ο�: https://www.cnblogs.com/qlky/p/7735145.html
//���: ��Ȼ�ǽ����㷨�����Ƴ���ָ�����ֵ�ƽ����
float Sqrt(float X)
{
	float HalfX = 0.5f * X;             // ��double���͵�������Ч
	int I = *(int*)&X;                  // get bits for floating VALUE 
	I = 0x5f375a86 - (I >> 1);          // gives initial guess y0
	X = *(float*)&I;                    // convert bits BACK to float
	X = X * (1.5f - HalfX * X * X);     // Newton step, repeating increases accuracy
	X = X * (1.5f - HalfX * X * X);     // Newton step, repeating increases accuracy
	X = X * (1.5f - HalfX * X * X);     // Newton step, repeating increases accuracy
	return 1 / X;
}

//����14: �޷��Ŷ�����ֱ��ͼ������ӣ�����Y = X + Y
//�ο�: ��
//���: SSE�Ż�
void HistgramAddShort(unsigned short *X, unsigned short *Y)
{
	*(__m128i*)(Y + 0) = _mm_add_epi16(*(__m128i*)&Y[0], *(__m128i*)&X[0]);		//	��Ҫ�������Լ�д�Ļ�೬�������ٶ��ˣ��Ѿ��Թ���
	*(__m128i*)(Y + 8) = _mm_add_epi16(*(__m128i*)&Y[8], *(__m128i*)&X[8]);
	*(__m128i*)(Y + 16) = _mm_add_epi16(*(__m128i*)&Y[16], *(__m128i*)&X[16]);
	*(__m128i*)(Y + 24) = _mm_add_epi16(*(__m128i*)&Y[24], *(__m128i*)&X[24]);
	*(__m128i*)(Y + 32) = _mm_add_epi16(*(__m128i*)&Y[32], *(__m128i*)&X[32]);
	*(__m128i*)(Y + 40) = _mm_add_epi16(*(__m128i*)&Y[40], *(__m128i*)&X[40]);
	*(__m128i*)(Y + 48) = _mm_add_epi16(*(__m128i*)&Y[48], *(__m128i*)&X[48]);
	*(__m128i*)(Y + 56) = _mm_add_epi16(*(__m128i*)&Y[56], *(__m128i*)&X[56]);
	*(__m128i*)(Y + 64) = _mm_add_epi16(*(__m128i*)&Y[64], *(__m128i*)&X[64]);
	*(__m128i*)(Y + 72) = _mm_add_epi16(*(__m128i*)&Y[72], *(__m128i*)&X[72]);
	*(__m128i*)(Y + 80) = _mm_add_epi16(*(__m128i*)&Y[80], *(__m128i*)&X[80]);
	*(__m128i*)(Y + 88) = _mm_add_epi16(*(__m128i*)&Y[88], *(__m128i*)&X[88]);
	*(__m128i*)(Y + 96) = _mm_add_epi16(*(__m128i*)&Y[96], *(__m128i*)&X[96]);
	*(__m128i*)(Y + 104) = _mm_add_epi16(*(__m128i*)&Y[104], *(__m128i*)&X[104]);
	*(__m128i*)(Y + 112) = _mm_add_epi16(*(__m128i*)&Y[112], *(__m128i*)&X[112]);
	*(__m128i*)(Y + 120) = _mm_add_epi16(*(__m128i*)&Y[120], *(__m128i*)&X[120]);
	*(__m128i*)(Y + 128) = _mm_add_epi16(*(__m128i*)&Y[128], *(__m128i*)&X[128]);
	*(__m128i*)(Y + 136) = _mm_add_epi16(*(__m128i*)&Y[136], *(__m128i*)&X[136]);
	*(__m128i*)(Y + 144) = _mm_add_epi16(*(__m128i*)&Y[144], *(__m128i*)&X[144]);
	*(__m128i*)(Y + 152) = _mm_add_epi16(*(__m128i*)&Y[152], *(__m128i*)&X[152]);
	*(__m128i*)(Y + 160) = _mm_add_epi16(*(__m128i*)&Y[160], *(__m128i*)&X[160]);
	*(__m128i*)(Y + 168) = _mm_add_epi16(*(__m128i*)&Y[168], *(__m128i*)&X[168]);
	*(__m128i*)(Y + 176) = _mm_add_epi16(*(__m128i*)&Y[176], *(__m128i*)&X[176]);
	*(__m128i*)(Y + 184) = _mm_add_epi16(*(__m128i*)&Y[184], *(__m128i*)&X[184]);
	*(__m128i*)(Y + 192) = _mm_add_epi16(*(__m128i*)&Y[192], *(__m128i*)&X[192]);
	*(__m128i*)(Y + 200) = _mm_add_epi16(*(__m128i*)&Y[200], *(__m128i*)&X[200]);
	*(__m128i*)(Y + 208) = _mm_add_epi16(*(__m128i*)&Y[208], *(__m128i*)&X[208]);
	*(__m128i*)(Y + 216) = _mm_add_epi16(*(__m128i*)&Y[216], *(__m128i*)&X[216]);
	*(__m128i*)(Y + 224) = _mm_add_epi16(*(__m128i*)&Y[224], *(__m128i*)&X[224]);
	*(__m128i*)(Y + 232) = _mm_add_epi16(*(__m128i*)&Y[232], *(__m128i*)&X[232]);
	*(__m128i*)(Y + 240) = _mm_add_epi16(*(__m128i*)&Y[240], *(__m128i*)&X[240]);
	*(__m128i*)(Y + 248) = _mm_add_epi16(*(__m128i*)&Y[248], *(__m128i*)&X[248]);
}

//����15: �޷��Ŷ�����ֱ��ͼ�������������Y = Y - X
//�ο�: ��
//���: SSE�Ż�
void HistgramSubShort(unsigned short *X, unsigned short *Y)
{
	*(__m128i*)(Y + 0) = _mm_sub_epi16(*(__m128i*)&Y[0], *(__m128i*)&X[0]);
	*(__m128i*)(Y + 8) = _mm_sub_epi16(*(__m128i*)&Y[8], *(__m128i*)&X[8]);
	*(__m128i*)(Y + 16) = _mm_sub_epi16(*(__m128i*)&Y[16], *(__m128i*)&X[16]);
	*(__m128i*)(Y + 24) = _mm_sub_epi16(*(__m128i*)&Y[24], *(__m128i*)&X[24]);
	*(__m128i*)(Y + 32) = _mm_sub_epi16(*(__m128i*)&Y[32], *(__m128i*)&X[32]);
	*(__m128i*)(Y + 40) = _mm_sub_epi16(*(__m128i*)&Y[40], *(__m128i*)&X[40]);
	*(__m128i*)(Y + 48) = _mm_sub_epi16(*(__m128i*)&Y[48], *(__m128i*)&X[48]);
	*(__m128i*)(Y + 56) = _mm_sub_epi16(*(__m128i*)&Y[56], *(__m128i*)&X[56]);
	*(__m128i*)(Y + 64) = _mm_sub_epi16(*(__m128i*)&Y[64], *(__m128i*)&X[64]);
	*(__m128i*)(Y + 72) = _mm_sub_epi16(*(__m128i*)&Y[72], *(__m128i*)&X[72]);
	*(__m128i*)(Y + 80) = _mm_sub_epi16(*(__m128i*)&Y[80], *(__m128i*)&X[80]);
	*(__m128i*)(Y + 88) = _mm_sub_epi16(*(__m128i*)&Y[88], *(__m128i*)&X[88]);
	*(__m128i*)(Y + 96) = _mm_sub_epi16(*(__m128i*)&Y[96], *(__m128i*)&X[96]);
	*(__m128i*)(Y + 104) = _mm_sub_epi16(*(__m128i*)&Y[104], *(__m128i*)&X[104]);
	*(__m128i*)(Y + 112) = _mm_sub_epi16(*(__m128i*)&Y[112], *(__m128i*)&X[112]);
	*(__m128i*)(Y + 120) = _mm_sub_epi16(*(__m128i*)&Y[120], *(__m128i*)&X[120]);
	*(__m128i*)(Y + 128) = _mm_sub_epi16(*(__m128i*)&Y[128], *(__m128i*)&X[128]);
	*(__m128i*)(Y + 136) = _mm_sub_epi16(*(__m128i*)&Y[136], *(__m128i*)&X[136]);
	*(__m128i*)(Y + 144) = _mm_sub_epi16(*(__m128i*)&Y[144], *(__m128i*)&X[144]);
	*(__m128i*)(Y + 152) = _mm_sub_epi16(*(__m128i*)&Y[152], *(__m128i*)&X[152]);
	*(__m128i*)(Y + 160) = _mm_sub_epi16(*(__m128i*)&Y[160], *(__m128i*)&X[160]);
	*(__m128i*)(Y + 168) = _mm_sub_epi16(*(__m128i*)&Y[168], *(__m128i*)&X[168]);
	*(__m128i*)(Y + 176) = _mm_sub_epi16(*(__m128i*)&Y[176], *(__m128i*)&X[176]);
	*(__m128i*)(Y + 184) = _mm_sub_epi16(*(__m128i*)&Y[184], *(__m128i*)&X[184]);
	*(__m128i*)(Y + 192) = _mm_sub_epi16(*(__m128i*)&Y[192], *(__m128i*)&X[192]);
	*(__m128i*)(Y + 200) = _mm_sub_epi16(*(__m128i*)&Y[200], *(__m128i*)&X[200]);
	*(__m128i*)(Y + 208) = _mm_sub_epi16(*(__m128i*)&Y[208], *(__m128i*)&X[208]);
	*(__m128i*)(Y + 216) = _mm_sub_epi16(*(__m128i*)&Y[216], *(__m128i*)&X[216]);
	*(__m128i*)(Y + 224) = _mm_sub_epi16(*(__m128i*)&Y[224], *(__m128i*)&X[224]);
	*(__m128i*)(Y + 232) = _mm_sub_epi16(*(__m128i*)&Y[232], *(__m128i*)&X[232]);
	*(__m128i*)(Y + 240) = _mm_sub_epi16(*(__m128i*)&Y[240], *(__m128i*)&X[240]);
	*(__m128i*)(Y + 248) = _mm_sub_epi16(*(__m128i*)&Y[248], *(__m128i*)&X[248]);
}

//����16: �޷��Ŷ�����ֱ��ͼ������Ӽ�������Z = Z + Y - X
//�ο�: ��
//���: SSE�Ż�
void HistgramSubAddShort(unsigned short *X, unsigned short *Y, unsigned short *Z)
{
	*(__m128i*)(Z + 0) = _mm_sub_epi16(_mm_add_epi16(*(__m128i*)&Y[0], *(__m128i*)&Z[0]), *(__m128i*)&X[0]);						//	��Ҫ�������Լ�д�Ļ�೬�������ٶ��ˣ��Ѿ��Թ���
	*(__m128i*)(Z + 8) = _mm_sub_epi16(_mm_add_epi16(*(__m128i*)&Y[8], *(__m128i*)&Z[8]), *(__m128i*)&X[8]);
	*(__m128i*)(Z + 16) = _mm_sub_epi16(_mm_add_epi16(*(__m128i*)&Y[16], *(__m128i*)&Z[16]), *(__m128i*)&X[16]);
	*(__m128i*)(Z + 24) = _mm_sub_epi16(_mm_add_epi16(*(__m128i*)&Y[24], *(__m128i*)&Z[24]), *(__m128i*)&X[24]);
	*(__m128i*)(Z + 32) = _mm_sub_epi16(_mm_add_epi16(*(__m128i*)&Y[32], *(__m128i*)&Z[32]), *(__m128i*)&X[32]);
	*(__m128i*)(Z + 40) = _mm_sub_epi16(_mm_add_epi16(*(__m128i*)&Y[40], *(__m128i*)&Z[40]), *(__m128i*)&X[40]);
	*(__m128i*)(Z + 48) = _mm_sub_epi16(_mm_add_epi16(*(__m128i*)&Y[48], *(__m128i*)&Z[48]), *(__m128i*)&X[48]);
	*(__m128i*)(Z + 56) = _mm_sub_epi16(_mm_add_epi16(*(__m128i*)&Y[56], *(__m128i*)&Z[56]), *(__m128i*)&X[56]);
	*(__m128i*)(Z + 64) = _mm_sub_epi16(_mm_add_epi16(*(__m128i*)&Y[64], *(__m128i*)&Z[64]), *(__m128i*)&X[64]);
	*(__m128i*)(Z + 72) = _mm_sub_epi16(_mm_add_epi16(*(__m128i*)&Y[72], *(__m128i*)&Z[72]), *(__m128i*)&X[72]);
	*(__m128i*)(Z + 80) = _mm_sub_epi16(_mm_add_epi16(*(__m128i*)&Y[80], *(__m128i*)&Z[80]), *(__m128i*)&X[80]);
	*(__m128i*)(Z + 88) = _mm_sub_epi16(_mm_add_epi16(*(__m128i*)&Y[88], *(__m128i*)&Z[88]), *(__m128i*)&X[88]);
	*(__m128i*)(Z + 96) = _mm_sub_epi16(_mm_add_epi16(*(__m128i*)&Y[96], *(__m128i*)&Z[96]), *(__m128i*)&X[96]);
	*(__m128i*)(Z + 104) = _mm_sub_epi16(_mm_add_epi16(*(__m128i*)&Y[104], *(__m128i*)&Z[104]), *(__m128i*)&X[104]);
	*(__m128i*)(Z + 112) = _mm_sub_epi16(_mm_add_epi16(*(__m128i*)&Y[112], *(__m128i*)&Z[112]), *(__m128i*)&X[112]);
	*(__m128i*)(Z + 120) = _mm_sub_epi16(_mm_add_epi16(*(__m128i*)&Y[120], *(__m128i*)&Z[120]), *(__m128i*)&X[120]);
	*(__m128i*)(Z + 128) = _mm_sub_epi16(_mm_add_epi16(*(__m128i*)&Y[128], *(__m128i*)&Z[128]), *(__m128i*)&X[128]);
	*(__m128i*)(Z + 136) = _mm_sub_epi16(_mm_add_epi16(*(__m128i*)&Y[136], *(__m128i*)&Z[136]), *(__m128i*)&X[136]);
	*(__m128i*)(Z + 144) = _mm_sub_epi16(_mm_add_epi16(*(__m128i*)&Y[144], *(__m128i*)&Z[144]), *(__m128i*)&X[144]);
	*(__m128i*)(Z + 152) = _mm_sub_epi16(_mm_add_epi16(*(__m128i*)&Y[152], *(__m128i*)&Z[152]), *(__m128i*)&X[152]);
	*(__m128i*)(Z + 160) = _mm_sub_epi16(_mm_add_epi16(*(__m128i*)&Y[160], *(__m128i*)&Z[160]), *(__m128i*)&X[160]);
	*(__m128i*)(Z + 168) = _mm_sub_epi16(_mm_add_epi16(*(__m128i*)&Y[168], *(__m128i*)&Z[168]), *(__m128i*)&X[168]);
	*(__m128i*)(Z + 176) = _mm_sub_epi16(_mm_add_epi16(*(__m128i*)&Y[176], *(__m128i*)&Z[176]), *(__m128i*)&X[176]);
	*(__m128i*)(Z + 184) = _mm_sub_epi16(_mm_add_epi16(*(__m128i*)&Y[184], *(__m128i*)&Z[184]), *(__m128i*)&X[184]);
	*(__m128i*)(Z + 192) = _mm_sub_epi16(_mm_add_epi16(*(__m128i*)&Y[192], *(__m128i*)&Z[192]), *(__m128i*)&X[192]);
	*(__m128i*)(Z + 200) = _mm_sub_epi16(_mm_add_epi16(*(__m128i*)&Y[200], *(__m128i*)&Z[200]), *(__m128i*)&X[200]);
	*(__m128i*)(Z + 208) = _mm_sub_epi16(_mm_add_epi16(*(__m128i*)&Y[208], *(__m128i*)&Z[208]), *(__m128i*)&X[208]);
	*(__m128i*)(Z + 216) = _mm_sub_epi16(_mm_add_epi16(*(__m128i*)&Y[216], *(__m128i*)&Z[216]), *(__m128i*)&X[216]);
	*(__m128i*)(Z + 224) = _mm_sub_epi16(_mm_add_epi16(*(__m128i*)&Y[224], *(__m128i*)&Z[224]), *(__m128i*)&X[224]);
	*(__m128i*)(Z + 232) = _mm_sub_epi16(_mm_add_epi16(*(__m128i*)&Y[232], *(__m128i*)&Z[232]), *(__m128i*)&X[232]);
	*(__m128i*)(Z + 240) = _mm_sub_epi16(_mm_add_epi16(*(__m128i*)&Y[240], *(__m128i*)&Z[240]), *(__m128i*)&X[240]);
	*(__m128i*)(Z + 248) = _mm_sub_epi16(_mm_add_epi16(*(__m128i*)&Y[248], *(__m128i*)&Z[248]), *(__m128i*)&X[248]);
}

//����17: ����Alphaͨ��������
//�ο�: ��
//���: ֱ����ԭʼ�Ĵ��룬�ٶȺܺ�
void CopyAlphaChannel(TMatrix *Src, TMatrix *Dest) {
	if (Src->Channel != 4 || Dest->Channel != 4) return;
	if (Src->Data == Dest->Data) return;
	unsigned char *SrcP = Src->Data, *DestP = Dest->Data;
	int Y, Index = 3;
	for (Y = 0; Y < Src->Width * Src->Height; Y++, Index += 4) {
		SrcP[Index] = DestP[Index];
	}
}

// ����18: ����ָ���ı�Եģʽ������չ������������ֵ
// �����б�: 
// Width: ����Ŀ��
// Height: ����ĸ߶�
// Left: �����Ҫ��չ������
// Right: �Ҳ���Ҫ��չ������
// Top: ������Ҫ��չ������
// Bottom: �ײ���Ҫ��չ������
// Edge: �����Ե�ķ�ʽ
// RawPos: �����з������������ֵ
// ColPos: �����з������������ֵ
// ���غ�����ִ�гɹ�
IS_RET GetValidCoordinate(int Width, int Height, int Left, int Right, int Top, int Bottom, EdgeMode Edge, TMatrix **Row, TMatrix **Col) {
	if((Left < 0) || (Right < 0) || (Top < 0) || (Bottom < 0)) return IS_RET_ERR_ARGUMENTOUTOFRANGE;
	IS_RET Ret = IS_CreateMatrix(Width + Left + Right, 1, IS_DEPTH_32S, 1, Row);
	if (Ret != IS_RET_OK) return Ret;
	Ret = IS_CreateMatrix(1, Height + Top + Bottom, IS_DEPTH_32S, 1, Col);
	if (Ret != IS_RET_OK) return Ret;
	int X, Y, XX, YY, *RowPos = (int *)(*Row)->Data, *ColPos = (int*)(*Col)->Data;
	for (X = -Left; X < Width + Right; X++) {
		if (X < 0) {
			if (Edge == EdgeMode::Tile) { //�ظ���Ե����
				RowPos[X + Left] = 0;
			}
			else {
				XX = -X;
				while (XX >= Width) XX -= Width;
				RowPos[X + Left] = XX;
			}
		}
	}
}