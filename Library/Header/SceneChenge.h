#pragma once

class SceneChenger
{
public: // �T�u�N���X
	enum Scene
	{
		Title,
		Game,
		Setting,
		StageEditer
	};

public: // �����o�֐�
	SceneChenger() {}
	~SceneChenger() {}

	virtual void SceneChenge(Scene scene, bool stackClear) = 0;
	virtual int PopScene() = 0;
};
