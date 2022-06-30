#pragma once
#include "./Header/BaseScene.h"

class SettingScene : public BaseScene
{
public: //�萔
	static const std::wstring settingResourcesDir;

public: //�����o�֐�
	SettingScene(SceneChenger* sceneChenger);
	~SettingScene() override;

	void Init() override;
	void Update() override;
	void Draw() override;

private: //�����o�ϐ�
	int background; //�w�i�摜
};
