#pragma once

class SceneChenger
{
public: // サブクラス
	enum Scene
	{
		Title,
		Game,
		Setting,
		StageEditer
	};

public: // メンバ関数
	SceneChenger() {}
	~SceneChenger() {}

	virtual void SceneChenge(Scene scene, bool stackClear) = 0;
	virtual int PopScene() = 0;
};
