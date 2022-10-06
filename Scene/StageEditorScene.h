#pragma once
#include "./Header/BaseScene.h"
#include "../Stage/Area.h"

class StageEditorScene : public BaseScene
{
public: //�萔
	static const int STAGE_WIDTH = 15;
	static const int STAGE_HEIGHT = 7;
	static const std::wstring resourcesDir;

public: //�ÓI�����o�ϐ�

private: //�����o�ϐ�
	Area stage;
	int mapArray[STAGE_WIDTH * STAGE_HEIGHT];
	int mapIndex;
	int blockIndex;

	int background; //�w�i�摜
	int cursor; //�J�[�\���摜

public: //�����o�֐�
	StageEditorScene(DrawPolygon* draw, SceneChenger* sceneChenger);
	~StageEditorScene() override;

	void Init() override;
	void Update() override;
	void Draw() override;
};
