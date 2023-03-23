// 色のテンプレート定数

#pragma once
#include <DirectXMath.h>

// アルファ値無し版
namespace Engine
{
namespace Color
{
constexpr DirectX::XMFLOAT3 WHITE       = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
constexpr DirectX::XMFLOAT3 BLACK       = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
constexpr DirectX::XMFLOAT3 RED         = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
constexpr DirectX::XMFLOAT3 GREEN       = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
constexpr DirectX::XMFLOAT3 BLUE        = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
constexpr DirectX::XMFLOAT3 YELLOW      = DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f);
constexpr DirectX::XMFLOAT3 MAGENTA     = DirectX::XMFLOAT3(1.0f, 0.0f, 1.0f);
constexpr DirectX::XMFLOAT3 CYAN        = DirectX::XMFLOAT3(0.0f, 1.0f, 1.0f);
constexpr DirectX::XMFLOAT3 ORANGE      = DirectX::XMFLOAT3(1.0f, 0.5f, 0.0f);
constexpr DirectX::XMFLOAT3 LIGHT_GREEN = DirectX::XMFLOAT3(0.5f, 1.0f, 0.0f);

// アルファ値の情報を付加する
constexpr DirectX::XMFLOAT4 AddAlphaValue(const DirectX::XMFLOAT3& rgb, float a = 1.0f) { return { rgb.x, rgb.y, rgb.z, a }; }
} //Color
} //Engine
