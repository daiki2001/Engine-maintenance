#pragma once

class SceneChanger
{
public: // サブクラス
	enum Scene
	{
		Title,
		Game,
		Setting,
		StageEditer,
		StageSelect
	};

public: // メンバ関数
	SceneChanger() {}
	~SceneChanger() {}

	virtual void SceneChange(Scene scene, bool stackClear) = 0;
	virtual int PopScene() = 0;
};
