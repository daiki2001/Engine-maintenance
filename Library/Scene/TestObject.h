#pragma once
#include "./Header/DirectDrawing.h"
#include "./Header/DrawPolygon.h"

class TestObject final : private OBJData
{
public: // シングルトン化
	static TestObject* Get();
private:
	TestObject();
	TestObject(const TestObject&) = delete;
	~TestObject() override;
	TestObject operator=(const TestObject&) = delete;

public: // 定数
	static const float sphereRadius; //オブジェクトの大きさ

public: // メンバ関数
	// 初期化
	void Init() override;
	// 更新
	void Update() override;
	// 描画
	void Draw();

	// 衝突時コールバック関数
	void OnCollision(const CollisionInfo& info) override;

	int CreateModel(const char* filePath = nullptr, const char* directoryPath = nullptr);
	inline void SetModel(const int& objectData) { object = objectData; }
	inline void SetDraw(DrawPolygon* draw) { this->draw = draw; }

private: // メンバ変数
	DrawPolygon* draw; //描画用オブジェクト

	int object; //オブジェクト
	bool isObj;
};
