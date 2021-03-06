// インライン関数
#pragma once
#include <Windows.h>
#define _USE_MATH_DEFINES
#include <math.h>
//#define _XM_NO_INTRINSICS_
#include <directxmath.h>
#include <string>

using namespace DirectX;
using namespace std;

// デバッグ用
#define _CRTDBG_MAP_ALLOC
#if defined(_DEBUG) && defined(_CRTDBG_MAP_ALLOC) && !defined(NEW)
#define NEW  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
#else
#define NEW  new
#endif

#define SAFE_RELEASE(x) if( x != nullptr ){ x->Release(); x = nullptr; }
#define SAFE_DELETE(x)  if( x != nullptr ){ delete x;  x = nullptr; }
#define SAFE_DELETE_ARRAY(x)  if( x != nullptr ){ delete[] x;  x = nullptr; }
#define SAFE_FREE(x)  if( x != nullptr ){ free( x );  x = nullptr; }

// デグリーをラジアンに変換
inline double RADIAN(double degree)
{
	return degree * M_PI / 180.0;
}

// ラジアンをデグリーに変換
inline double DEGREE(double radian)
{
	return radian * 180.0 / M_PI;
}

// デグリーをラジアンに変換ふろーとばん
inline float RADIAN(float degree)
{
	return (float)(degree * M_PI / 180.0);
}

// ラジアンをデグリーに変換ふろーとばｎ
inline float DEGREE(float radian)
{
	return (float)(radian * 180.0 / M_PI);
}

// メッセージボックスを表示します。
// windowname ウィンドウの名前　　val メッセージボックスの文章
inline void MSGBOX(LPCTSTR windowname, LPCTSTR val)
{
	MessageBox(nullptr, windowname, val, (MB_OK | MB_ICONWARNING));
}

template <typename T>
inline int ARRAY_NUM(T *&p) {
	if (p) {
		return sizeof(p) / sizeof(p[0]);
	}
	else
	{
		return 0;
	}
}

template <typename T>
inline int XOR_SWAP(T *a, T *b) {
	*a ^= *b;
	*b ^= *a;
	*a ^= *b;
}

inline DWORD XOR_RAND() {
	static DWORD x = 123456789; // い　な
	static DWORD y = 362436069; // い　ん
	static DWORD z = 521288629; // で　で
	static DWORD w = 88675123;  // す　も
	DWORD t;

	t = x ^ (x << 11);
	x = y; y = z; z = w;
	return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
}

inline float XMVector3LengthSq(XMFLOAT3* f)
{
	float a;
	XMVECTOR vec = XMVector3LengthSq(XMLoadFloat3(f));
	XMStoreFloat(&a, vec);

	return a;
}

inline XMFLOAT3 MatrixTransformPosition(XMFLOAT4X4* mat)
{
	XMFLOAT3 pos;

	pos.x = mat->_41;
	pos.y = mat->_42;
	pos.z = mat->_43;

	return pos;
}


inline XMFLOAT3 MatrixTransformScale(XMFLOAT4X4* mat)
{
	XMFLOAT3 scale;

	scale.x = mat->_11;
	scale.y = mat->_22;
	scale.z = mat->_33;

	return scale;
}

//拡張子取得関数
static string GetExtension(const string &path)
{
	string ext;
	size_t pos1 = path.rfind('.');
	if (pos1 != string::npos) {
		ext = path.substr(pos1 + 1, path.size() - pos1);
		string::iterator itr = ext.begin();
		while (itr != ext.end()) {
			*itr = tolower(*itr);
			itr++;
		}
		itr = ext.end() - 1;
		while (itr != ext.begin()) {    // パスの最後に\0やスペースがあったときの対策
			if (*itr == 0 || *itr == 32) {
				ext.erase(itr--);
			}
			else {
				itr--;
			}
		}
	}

	return ext;
}

static LPWSTR* StringToWideChar(LPWSTR* str, string temp)
{
	int n;
	n = MultiByteToWideChar(CP_ACP, 0, temp.c_str(), temp.size(), nullptr, 0);
	*str = NEW WCHAR[n + 1];
	n = MultiByteToWideChar(CP_ACP, 0, temp.c_str(), temp.size(), *str, n);
	*(*str + n) = '\0';
	return str;
}


//ZXY回転　XMMatrixRotationRollPitchYawで作成した行列
//mtx = XMMatrixRotationRollPitchYaw(rx,ry,rz);
//mtxからオイラー角を求める
//ただし -π/2 < rx < +π/2
static XMFLOAT3 MatrixToEulerZXY(XMMATRIX& mat)
{
	XMFLOAT3 rot;
	rot.x = -asinf(XMVectorGetY(mat.r[2]));
	rot.y = atan2f(XMVectorGetX(mat.r[2]), XMVectorGetZ(mat.r[2]));
	rot.z = atan2f(XMVectorGetY(mat.r[0]), XMVectorGetY(mat.r[1]));

	return rot;
}


//ZYX回転
//mtx = XMMatrixRotationZ(rz);
//mtx = XMMatrixMultiply(mtx, XMMatrixRotationY( ry ));
//mtx = XMMatrixMultiply(mtx, XMMatrixRotationX( rx ));
//mtxからオイラー角を求める
//ただし -π/2 < ry < +π/2
static XMFLOAT3 MatrixToEulerZYX(XMMATRIX& mat)
{
	XMFLOAT3 rot;
	rot.x = -atan2f(XMVectorGetY(mat.r[2]), XMVectorGetZ(mat.r[2]));
	rot.y = asinf(XMVectorGetX(mat.r[2]));
	rot.z = -atan2f(XMVectorGetX(mat.r[1]), XMVectorGetX(mat.r[0]));

	return rot;
}


//XYZ回転
//mtx = XMMatrixRotationX(rx);
//mtx = XMMatrixMultiply(mtx, XMMatrixRotationY( ry ));
//mtx = XMMatrixMultiply(mtx, XMMatrixRotationZ( rz ));
//mtxからオイラー角を求める
//ただし -π/2 < ry < +π/2
static XMFLOAT3 MatrixToEulerXYZ(XMMATRIX& mat)
{
	XMFLOAT3 rot;
	rot.x = atan2f(XMVectorGetZ(mat.r[1]), XMVectorGetZ(mat.r[2]));
	rot.y = -asinf(XMVectorGetZ(mat.r[0]));
	rot.z = atan2f(XMVectorGetY(mat.r[0]), XMVectorGetX(mat.r[0]));

	return rot;
}