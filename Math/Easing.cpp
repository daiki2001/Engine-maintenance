#include "Easing.h"

namespace Engine
{
namespace Math
{
// 線形補間
const Vector3 Lerp(const Vector3& start, const Vector3& end, float t)
{
	// float y = t;
	// return start * (1.0f - y) + end * y;
	return start * (1.0f - t) + end * t;
}

// easeIn補間
const Vector3 easeIn(const Vector3& start, const Vector3& end, float t)
{
	float y = t * t;
	return start * (1.0f - y) + end * y;
}

// easeOut補間
const Vector3 easeOut(const Vector3& start, const Vector3& end, float t)
{
	float y = t * (2 - t);
	return start * (1.0f - y) + end * y;
}

// easeInOut補間
const Vector3 easeInOut(const Vector3& start, const Vector3& end, float t)
{
	float y = t * t * (3 - 2 * t);
	return start * (1.0f - y) + end * y;
}
} //Math
} //Engine
