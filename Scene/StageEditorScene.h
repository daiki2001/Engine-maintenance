#pragma once
#include "./Header/BaseScene.h"

class StageEditorScene : public BaseScene
{
public: //�萔
	static const std::wstring resourcesDir;

public: //�����o�֐�
	StageEditorScene(DrawPolygon* draw, SceneChenger* sceneChenger);
	~StageEditorScene() override;

	void Init() override;
	void Update() override;
	void Draw() override;

private: //�����o�ϐ�
	int background; //�w�i�摜
};
