#pragma once
#include "SceneChange.h"
#include "BaseScene.h"
#include <memory>
#include <stack>

class SceneManager : public SceneChanger
{
public: // メンバ関数
	SceneManager() = delete;
	SceneManager(DrawPolygon* draw);
	~SceneManager() {}

	void Loop() const;
	void SceneChange(SceneChanger::Scene scene, bool stackClear) override;
	int PopScene() override;

private: // メンバ変数
	DrawPolygon* draw;
	std::stack<std::shared_ptr<BaseScene>> sceneStack;
};
