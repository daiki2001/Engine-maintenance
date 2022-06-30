#pragma once
#include "./Header/BaseScene.h"
#include <string>

class TitleScene : public BaseScene
{
public: //�萔
	static const std::wstring titleResourcesDir;

public: //�����o�֐�
	TitleScene(SceneChenger* sceneChenger);
	~TitleScene() override;

	void Init() override;
	void Update() override;
	void Draw() override;

private: //�����o�ϐ�
	int background; //�w�i�摜
};
