#pragma once
#include "./Header/BaseScene.h"
#include "./Header/EngineGeneral.h"
#include "./Math/Collision/Collision.h"
#include "TestObject.h"
#include "./Header/FbxLoader.h"
#include "./Header/Model.h"
#include "./Header/PostEffect.h"

#include <memory>

class EngineTestScene : public BaseScene
{
public: // エイリアス
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using Vector3 = Math::Vector3;

public: // 定数

public: // メンバ関数
	EngineTestScene(SceneChenger* sceneChenger);
	~EngineTestScene() override;

	void Init() override;
	void Update() override;
	void Draw() override;

private: // メンバ変数
	int background; //背景画像

	PostEffect postEffect;
	FbxLoader* fbxLoader;

	// オブジェクトの生成
	int fbxModel; //FBXモデル
	int fbxModel2; //FBXモデル

	UINT useShader;
	int defaultPostEffect;
};
