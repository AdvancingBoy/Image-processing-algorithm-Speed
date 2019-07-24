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
		else if (X >= Width) {
			if (Edge == EdgeMode::Tile) {
				RowPos[X + Left] = 0;
			}
			else {
				XX = -X;
				while (XX >= Width) XX -= Width; //����������
				RowPos[X + Left] = XX;
			}
		}
		else {
			RowPos[X + Left] = X;
		}
	}
	for (Y = -Top; Y < Height + Bottom; Y++) {
		if (Y < 0) {
			if (Edge == EdgeMode::Tile)
				ColPos[Y + Top] = 0;
			else {
				YY = -Y;
				while (YY >= Height) YY -= Height;
				ColPos[Y + Top] = YY;
			}
		}
		else if (Y >= Height) {
			if (Edge == EdgeMode::Tile)
				ColPos[Y + Top] = Height - 1;
			else {
				YY = Height - (Y - Height + 2);
				while (YY < 0) YY += Height;
				ColPos[Y + Top] = YY;
			}
		}
		else {
			ColPos[Y + Top] = Y;
		}
	}
	return IS_RET_OK;
}

// ����19: ����ɫͼ��ֽ�ΪR��G��B��A��ͨ����ͼ��
// �����б�:
// Src: ��Ҫ�����Դͼ������ݽṹ
// Blue: ����Blueͨ��ͼ������ݽṹ
// Green: ����Greenͨ��ͼ������ݽṹ
// Red: ����Redͨ��ͼ������ݽṹ
// Alpha: ����Alphaͨ��ͼ������ݽṹ
// ������8λ���д����ٶȴ������20%
// ���غ����Ƿ�ִ�гɹ�
IS_RET SplitRGBA(TMatrix *Src, TMatrix **Blue, TMatrix **Green, TMatrix **Red, TMatrix **Alpha) {
	if (Src == NULL) return IS_RET_ERR_NULLREFERENCE;
	if (Src->Data == NULL) return IS_RET_ERR_NULLREFERENCE;
	if (Src->Depth != IS_DEPTH_8U) return IS_RET_ERR_NOTSUPPORTED;
	IS_RET Ret = IS_CreateMatrix(Src->Width, Src->Height, Src->Depth, 1, Blue);
	if (Ret != IS_RET_OK) goto Done;
	Ret = IS_CreateMatrix(Src->Width, Src->Height, Src->Depth, 1, Green);
	if (Ret != IS_RET_OK) goto Done;
	Ret = IS_CreateMatrix(Src->Width, Src->Height, Src->Depth, 1, Red);
	if (Ret != IS_RET_OK) goto Done;
	if (Src->Channel == 4) {
		Ret = IS_CreateMatrix(Src->Width, Src->Height, Src->Depth, 1, Alpha);
		if (Ret != IS_RET_OK) goto Done;
	}
	int X, Y, Block, Width = Src->Width, Height = Src->Height;
	unsigned char *LinePS, *LinePB, *LinePG, *LinePR, *LinePA;
	const int BlockSize = 8;
	Block = Width / BlockSize;						//	8·����,�ٶ࿪��·���ٶȲ�û��������
	if (Src->Channel == 3)
	{
		for (Y = 0; Y < Height; Y++)
		{
			LinePS = Src->Data + Y * Src->WidthStep;
			LinePB = (*Blue)->Data + Y * (*Blue)->WidthStep;
			LinePG = (*Green)->Data + Y * (*Green)->WidthStep;
			LinePR = (*Red)->Data + Y * (*Red)->WidthStep;
			for (X = 0; X < Block * BlockSize; X += BlockSize)			//	�����LinePBȫд��һ���ٶȷ�����һЩ
			{
				LinePB[0] = LinePS[0];		LinePG[0] = LinePS[1];		LinePR[0] = LinePS[2];
				LinePB[1] = LinePS[3];		LinePG[1] = LinePS[4];		LinePR[1] = LinePS[5];
				LinePB[2] = LinePS[6];		LinePG[2] = LinePS[7];		LinePR[2] = LinePS[8];
				LinePB[3] = LinePS[9];		LinePG[3] = LinePS[10];		LinePR[3] = LinePS[11];
				LinePB[4] = LinePS[12];		LinePG[4] = LinePS[13];		LinePR[4] = LinePS[14];
				LinePB[5] = LinePS[15];		LinePG[5] = LinePS[16];		LinePR[5] = LinePS[17];
				LinePB[6] = LinePS[18];		LinePG[6] = LinePS[19];		LinePR[6] = LinePS[20];
				LinePB[7] = LinePS[21];		LinePG[7] = LinePS[22];		LinePR[7] = LinePS[23];
				LinePB += 8;				LinePG += 8;				LinePR += 8;				LinePS += 24;
			}
			while (X < Width)
			{
				LinePB[0] = LinePS[0];		LinePG[0] = LinePS[1];		LinePR[0] = LinePS[2];
				LinePB++;					LinePG++;					LinePR++;					LinePS += 3;
				X++;
			}
		}
	}
	else if (Src->Channel == 4)
	{
		for (Y = 0; Y < Height; Y++)
		{
			LinePS = Src->Data + Y * Src->WidthStep;
			LinePB = (*Blue)->Data + Y * (*Blue)->WidthStep;
			LinePG = (*Green)->Data + Y * (*Green)->WidthStep;
			LinePR = (*Red)->Data + Y * (*Red)->WidthStep;
			LinePA = (*Alpha)->Data + Y * (*Alpha)->WidthStep;
			for (X = 0; X < Block * BlockSize; X += BlockSize)
			{
				LinePB[0] = LinePS[0];		LinePG[0] = LinePS[1];		LinePR[0] = LinePS[2];		LinePA[0] = LinePS[3];
				LinePB[1] = LinePS[4];		LinePG[1] = LinePS[5];		LinePR[1] = LinePS[6];		LinePA[1] = LinePS[7];
				LinePB[2] = LinePS[8];		LinePG[2] = LinePS[9];		LinePR[2] = LinePS[10];		LinePA[2] = LinePS[11];
				LinePB[3] = LinePS[12];		LinePG[3] = LinePS[13];		LinePR[3] = LinePS[14];		LinePA[3] = LinePS[15];
				LinePB[4] = LinePS[16];		LinePG[4] = LinePS[17];		LinePR[4] = LinePS[18];		LinePA[4] = LinePS[19];
				LinePB[5] = LinePS[20];		LinePG[5] = LinePS[21];		LinePR[5] = LinePS[22];		LinePA[5] = LinePS[23];
				LinePB[6] = LinePS[24];		LinePG[6] = LinePS[25];		LinePR[6] = LinePS[26];		LinePA[6] = LinePS[27];
				LinePB[7] = LinePS[28];		LinePG[7] = LinePS[29];		LinePR[7] = LinePS[30];		LinePA[7] = LinePS[31];
				LinePB += 8;				LinePG += 8;				LinePR += 8;				LinePA += 8;				LinePS += 32;
			}
			while (X < Width)
			{
				LinePB[0] = LinePS[0];		LinePG[0] = LinePS[1];		LinePR[0] = LinePS[2];		LinePA[0] = LinePS[3];
				LinePB++;					LinePG++;					LinePR++;					LinePA++;					LinePS += 4;
				X++;
			}
		}
	}
	return IS_RET_OK;
Done:
	if (*Blue != NULL) IS_FreeMatrix(Blue);
	if (*Green != NULL) IS_FreeMatrix(Green);
	if (*Red != NULL) IS_FreeMatrix(Red);
	if (*Alpha != NULL) IS_FreeMatrix(Alpha);
	return Ret;
}

// ����20: ��R,G,B,A��ͨ����ͼ��ϲ�Ϊ��ɫ��ͼ��
// �����б�:
// Dest: �ϲ�������ͼ������ݽṹ
// Blue: Blueͨ��ͼ������ݽṹ
// Green: Greenͨ��ͼ������ݽṹ
// Red: Redͨ��ͼ������ݽṹ
// Alpha: Alphaͨ��ͼ������ݽṹ
IS_RET CombineRGBA(TMatrix *Dest, TMatrix *Blue, TMatrix *Green, TMatrix *Red, TMatrix *Alpha)
{
	if (Dest == NULL || Blue == NULL || Green == NULL || Red == NULL) return IS_RET_ERR_NULLREFERENCE;
	if (Dest->Data == NULL || Blue->Data == NULL || Green->Data == NULL || Red->Data == NULL) return IS_RET_ERR_NULLREFERENCE;
	if ((Dest->Channel != 3 && Dest->Channel != 4) || Blue->Channel != 1 || Green->Channel != 1 || Red->Channel != 1) return IS_RET_ERR_PARAMISMATCH;
	if (Dest->Width != Blue->Width || Dest->Width != Green->Width || Dest->Width != Red->Width || Dest->Width != Blue->Width)  return IS_RET_ERR_PARAMISMATCH;
	if (Dest->Height != Blue->Height || Dest->Height != Green->Height || Dest->Height != Red->Height || Dest->Height != Blue->Height)  return IS_RET_ERR_PARAMISMATCH;

	if (Dest->Channel == 4)
	{
		if (Alpha == NULL) return IS_RET_ERR_NULLREFERENCE;
		if (Alpha->Data == NULL) return IS_RET_ERR_NULLREFERENCE;
		if (Alpha->Channel != 1) return IS_RET_ERR_PARAMISMATCH;
		if (Dest->Width != Alpha->Width || Dest->Height != Alpha->Height) return IS_RET_ERR_PARAMISMATCH;
	}

	int X, Y, Block, Width = Dest->Width, Height = Dest->Height;
	unsigned char *LinePD, *LinePB, *LinePG, *LinePR, *LinePA;
	const int BlockSize = 8;
	Block = Width / BlockSize;						//	8·����,�ٶ࿪��·���ٶȲ�û��������

	if (Dest->Channel == 3)
	{
		for (Y = 0; Y < Height; Y++)
		{
			LinePD = Dest->Data + Y * Dest->WidthStep;
			LinePB = Blue->Data + Y * Blue->WidthStep;
			LinePG = Green->Data + Y * Green->WidthStep;
			LinePR = Red->Data + Y * Red->WidthStep;
			for (X = 0; X < Block * BlockSize; X += BlockSize)				//	�����LinePBȫд��һ���ٶ����𲻴�
			{
				LinePD[0] = LinePB[0];		LinePD[1] = LinePG[0];		LinePD[2] = LinePR[0];
				LinePD[3] = LinePB[1];		LinePD[4] = LinePG[1];		LinePD[5] = LinePR[1];
				LinePD[6] = LinePB[2];		LinePD[7] = LinePG[2];		LinePD[8] = LinePR[2];
				LinePD[9] = LinePB[3];		LinePD[10] = LinePG[3];		LinePD[11] = LinePR[3];
				LinePD[12] = LinePB[4];		LinePD[13] = LinePG[4];		LinePD[14] = LinePR[4];
				LinePD[15] = LinePB[5];		LinePD[16] = LinePG[5];		LinePD[17] = LinePR[5];
				LinePD[18] = LinePB[6];		LinePD[19] = LinePG[6];		LinePD[20] = LinePR[6];
				LinePD[21] = LinePB[7];		LinePD[22] = LinePG[7];		LinePD[23] = LinePR[7];
				LinePB += 8;				LinePG += 8;				LinePR += 8;				LinePD += 24;
			}
			while (X < Width)
			{
				LinePD[0] = LinePB[0];		LinePD[1] = LinePG[0];		LinePD[2] = LinePR[0];
				LinePB++;					LinePG++;					LinePR++;					LinePD += 3;
				X++;
			}
		}
	}
	else if (Dest->Channel == 4)
	{
		for (Y = 0; Y < Height; Y++)
		{
			LinePD = Dest->Data + Y * Dest->WidthStep;
			LinePB = Blue->Data + Y * Blue->WidthStep;
			LinePG = Green->Data + Y * Green->WidthStep;
			LinePR = Red->Data + Y * Red->WidthStep;
			LinePA = Alpha->Data + Y * Alpha->WidthStep;
			for (X = 0; X < Block * BlockSize; X += BlockSize)
			{
				LinePD[0] = LinePB[0];		LinePD[1] = LinePG[0];		LinePD[2] = LinePR[0];		LinePD[3] = LinePA[0];
				LinePD[4] = LinePB[1];		LinePD[5] = LinePG[1];		LinePD[6] = LinePR[1];		LinePD[7] = LinePA[1];
				LinePD[8] = LinePB[2];		LinePD[9] = LinePG[2];		LinePD[10] = LinePR[2];		LinePD[11] = LinePA[2];
				LinePD[12] = LinePB[3];		LinePD[13] = LinePG[3];		LinePD[14] = LinePR[3];		LinePD[15] = LinePA[3];
				LinePD[16] = LinePB[4];		LinePD[17] = LinePG[4];		LinePD[18] = LinePR[4];		LinePD[19] = LinePA[4];
				LinePD[20] = LinePB[5];		LinePD[21] = LinePG[5];		LinePD[22] = LinePR[5];		LinePD[23] = LinePA[5];
				LinePD[24] = LinePB[6];		LinePD[25] = LinePG[6];		LinePD[26] = LinePR[6];		LinePD[27] = LinePA[6];
				LinePD[28] = LinePB[7];		LinePD[29] = LinePG[7];		LinePD[30] = LinePR[7];		LinePD[31] = LinePA[7];
				LinePB += 8;				LinePG += 8;				LinePR += 8;				LinePA += 8;				LinePD += 32;
			}
			while (X < Width)
			{
				LinePD[0] = LinePB[0];		LinePD[1] = LinePG[0];		LinePD[2] = LinePR[0];		LinePD[3] = LinePA[0];
				LinePB++;					LinePG++;					LinePD++;					LinePA++;					LinePD += 4;
				X++;
			}
		}
	}
	return IS_RET_OK;
}