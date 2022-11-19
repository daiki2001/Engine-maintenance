#pragma once
#include "BaseScene.h"

class EngineTestScene : public BaseScene
{
public: // エイリアス

public: // 定数

public: // メンバ関数
	EngineTestScene(SceneChanger* sceneChanger);
	~EngineTestScene() override;

	void Init() override;
	void Update() override;
	void Draw() override;

private: // メンバ変数
	//int boxObject; //ボックスオブジェクト

};
