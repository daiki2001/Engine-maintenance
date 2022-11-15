#pragma once
#include "SceneChenge.h"
#include "BaseScene.h"
#include <memory>
#include <stack>

class SceneManager : public SceneChenger
{
public: // メンバ関数
	SceneManager() = delete;
	SceneManager(DrawPolygon* draw);
	~SceneManager() {}

	void Loop() const;
	void SceneChenge(SceneChenger::Scene scene, bool stackClear) override;
	int PopScene() override;

private: // メンバ変数
	DrawPolygon* draw;
	std::stack<std::shared_ptr<BaseScene>> sceneStack;
};
