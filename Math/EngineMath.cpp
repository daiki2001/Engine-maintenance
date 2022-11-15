#include "EngineMath.h"

namespace Engine
{
namespace Math
{
bool operator==(const DirectX::XMFLOAT2& a, const DirectX::XMFLOAT2& b)
{
	return a.x == b.x && a.y == b.y;
}

bool operator!=(const DirectX::XMFLOAT2& a, const DirectX::XMFLOAT2& b)
{
	return !(a == b);
}

bool operator==(const DirectX::XMFLOAT4& a, const DirectX::XMFLOAT4& b)
{
	return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

bool operator!=(const DirectX::XMFLOAT4& a, const DirectX::XMFLOAT4& b)
{
	return !(a == b);
}
} //Math
} //Engine