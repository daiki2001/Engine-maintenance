#pragma once
#include <DirectXMath.h>

// �C���N���[�h�t�@�C��
#include "./Math/Vector3.h"
#include "./Math/Matrix4.h"
#include "./Math/Easing.h"

/*�萔*/
namespace Engine
{
namespace Math
{
static constexpr double PI = 3.141592653589793;
static constexpr float PI_F = static_cast<float>(PI);
static constexpr double DEGREE = PI / 180.0;
static constexpr float DEGREE_F = static_cast<float>(DEGREE);

static constexpr float gravity = 9.8f; //�d�͉����x
} //Math
} //Engine

/*�֐�*/
namespace Engine
{
namespace Math
{
bool operator==(const DirectX::XMFLOAT4& a, const DirectX::XMFLOAT4& b);
bool operator!=(const DirectX::XMFLOAT4& a, const DirectX::XMFLOAT4& b);

template <class T>
T LockRatio(const float& x)
{
	return T();
}
} //Math
} //Engine
