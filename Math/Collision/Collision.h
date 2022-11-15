#pragma once
#include "CollisionPrimitive.h"
#include "./Math/EngineMath.h"

namespace Collision
{
using Vector3 = Engine::Math::Vector3;
using Matrix4 = Engine::Math::Matrix4;

// 球体同士の当たり判定
bool IsSphereToSphereCollision(const Sphere& sphere1, const Sphere& sphere2, Vector3* inter = nullptr);

// 球体と平面の当たり判定
bool IsSphereToPlaneCollision(const Sphere& sphere, const Plane& plane);

// 球体と三角形の当たり判定
bool IsSphereToTriangleCollision(const Sphere& sphere, const Triangle& triangle);

// レイと球体の当たり判定
bool IsRayToSphereCollision(const Ray& ray, const Sphere& sphere, float* distance = nullptr, Vector3* inter = nullptr);

// AABB同士の当たり判定
bool IsAABBToAABBCollision(const Vector3& leftUpFront1, const Vector3& rightDownBack1,
						   const Vector3& leftUpFront2, const Vector3& rightDownBack2);

// 球体とAABBの当たり判定
bool IsSphereToAABBCollision(const Sphere& sphere, const Vector3& leftUpFront, const Vector3& rightDownBack);

// レイと平面の当たり判定
bool IsRayToPlaneCollision(const Ray& ray, const Plane& plane, float* distance = nullptr, Vector3* inter = nullptr);

// レイと三角形の当たり判定
bool IsRayToTriangleCollision(const Ray& ray, const Triangle& triangle, float* distance = nullptr, Vector3* inter = nullptr);

// OBB同士の当たり判定
bool IsOBBToOBBCollision(const Vector3& pos1, const Matrix4& rotation1, const Vector3& scale1,
						 const Vector3& pos2, const Matrix4& rotation2, const Vector3& scale2);

// 球体スイープ
// 当たっていたら衝突した時間を0~1の間で返す、当たっていなかったら-1を返す
float sphereSwept(const Vector3& pos1, const Vector3& speed1, float r1,
				  const Vector3& pos2, const Vector3& speed2, float r2);

// 衝突予測
// 箱の場合は、Vector3(横幅, 高さ, 奥行).Length() / 2を入れれば使えます
bool IsPredictCollisionBall(const Vector3& pos1, const Vector3& speed1, float r1,
							const Vector3& pos2, const Vector3& speed2, float r2);
} //Collision
