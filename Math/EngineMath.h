#pragma once
#include <DirectXMath.h>

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

template <class T>
T LockRatio(float x)
{
	return T();
}
} //Math
} //Engine
