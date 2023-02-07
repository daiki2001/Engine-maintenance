#pragma once
#include <DirectXMath.h>
#include <cmath>

// インクルードファイル
#include "./Math/Vector3.h"
#include "./Math/Matrix4.h"
#include "./Math/Easing.h"

/*定数*/
namespace Engine
{
namespace Math
{
static constexpr double PI = 3.141592653589793;
static constexpr float PI_F = static_cast<float>(PI);
static constexpr double DEGREE = PI / 180.0;
static constexpr float DEGREE_F = static_cast<float>(DEGREE);

static constexpr float gravity = 9.8f; //重力加速度
} //Math
} //Engine

/*関数*/
namespace Engine
{
namespace Math
{
/*XMFLOAT2のオペレーター演算子のオーバーロード*/
// XMFLOAT2の==演算子
bool operator==(const DirectX::XMFLOAT2& a, const DirectX::XMFLOAT2& b);
// XMFLOAT2の!=演算子
bool operator!=(const DirectX::XMFLOAT2& a, const DirectX::XMFLOAT2& b);
/*XMFLOAT4のオペレーター演算子のオーバーロード*/
// XMFLOAT4の==演算子
bool operator==(const DirectX::XMFLOAT4& a, const DirectX::XMFLOAT4& b);
// XMFLOAT4の!=演算子
bool operator!=(const DirectX::XMFLOAT4& a, const DirectX::XMFLOAT4& b);
} //Math
} //Engine

/*テンプレート関数*/
namespace Engine
{
namespace Math
{
// 符号関数
template <class T>
T Sign(const T& x)
{
	if (x > 0)
	{
		return static_cast<T>(+1);
	}
	if (x < 0)
	{
		return static_cast<T>(-1);
	}
	return static_cast<T>(0);
}

// 四捨五入
// 33.3→33.0にしたい場合、multiplierに-1を指定してください
template <class T>
T RoundOff(const T& x, int multiplier = -1)
{
	if (x == static_cast<T>(0))
	{
		return x;
	}

	double multi = (static_cast<double>(multiplier) * (-1.0)) - 1.0;
	T y = x * std::pow(10, multi);
	y = std::round(y);
	y /= std::pow(10, multi);

	return y;
}
} //Math
} //Engine
