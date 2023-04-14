#pragma once
#include "SceneChange.h"
#include "SceneChangeAnimation.h"

class BaseScene
{
protected: //静的メンバ変数
	static SceneChangeAnimation changeAnimation;

public: //静的メンバ関数
	static void ChangeAnimationUpdate() { changeAnimation.Update(); }
	static void ChangeAnimationDraw() { changeAnimation.Draw(); }

protected: //メンバ変数
	SceneChanger* sceneChanger;

	bool isSceneDest;              //シーンを破棄するかどうか
	SceneChanger::Scene nextScene; //次のシーンの遷移先

public: //メンバ関数
	BaseScene() = delete;
	BaseScene(SceneChanger* sceneChanger);
	virtual ~BaseScene();

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void BGDraw() {} //背景描画
	virtual void UIDraw() {} //UI描画

	void ChengeTitleScene(bool flag);
};
