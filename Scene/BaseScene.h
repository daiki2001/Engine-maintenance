#pragma once
#include "./Header/DrawPolygon.h"
#include "SceneChange.h"
#include "SceneChangeAnimation.h"

class BaseScene
{
protected: //静的メンバ変数
	static DrawPolygon* draw;
	static SceneChangeAnimation changeAnimation;

public: //静的メンバ関数
	static void StaticInit(DrawPolygon* draw) {
		BaseScene::draw = draw;
		SceneChangeAnimation::StaticInit(BaseScene::draw);
	}
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

	void ChengeTitleScene(bool flag);
};
