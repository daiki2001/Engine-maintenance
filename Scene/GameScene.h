#pragma once
#include "./Header/BaseScene.h"

class GameScene :  public BaseScene
{
public: //�萔
	static const std::wstring gameResourcesDir;

public: //�����o�֐�
	GameScene(SceneChenger* sceneChenger);
	~GameScene() override;

	void Init() override;
	void Update() override;
	void Draw() override;

private: //�����o�ϐ�
	int background; //�w�i�摜
};
