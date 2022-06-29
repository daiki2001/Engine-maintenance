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
static constexpr float degree = DirectX::XM_PI / 180.0f; //1��

static constexpr float gravity = 9.8f; //�d�͉����x
} //Math
} //Engine

/*�I�y���[�^�[���Z�q�I�[�o�[���[�h*/
namespace Engine
{
namespace Math
{
bool operator==(const DirectX::XMFLOAT4& a, const DirectX::XMFLOAT4& b);
bool operator!=(const DirectX::XMFLOAT4& a, const DirectX::XMFLOAT4& b);
} //Math
} //Engine
