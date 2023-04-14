#pragma once
#include "SceneChange.h"
#include "BaseScene.h"
#include "./Header/PostEffect.h"
#include <memory>
#include <stack>

class SceneManager : public SceneChanger
{
private: //静的メンバ変数
	static PostEffect postEffect;

public: // メンバ関数
	SceneManager();
	~SceneManager() {}

	void Loop() const;
	void SceneChange(SceneChanger::Scene scene, bool stackClear) override;
	int PopScene() override;

private: // メンバ変数
	std::stack<std::shared_ptr<BaseScene>> sceneStack;
};
