#pragma once
#include "Vector3.h"

// 補間関数
namespace Engine
{
namespace Math
{
struct Ease
{
	Vector3 start = Vector3::Zero();
	Vector3 end = Vector3::Zero();
	float time = 0.0f;
	float addTime = 0.0f;
	bool isAlive = false;
};

// 線形補間(1次関数補間)
const Vector3 Lerp(const Vector3& start, const Vector3& end, float t);

// easeIn補間
const Vector3 easeIn(const Vector3& start, const Vector3& end, float t);
// easeOut補間
const Vector3 easeOut(const Vector3& start, const Vector3& end, float t);
// easeInOut補間
const Vector3 easeInOut(const Vector3& start, const Vector3& end, float t);
} //Math
} //Engine
