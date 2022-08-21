#pragma once
#include "./Header/BaseScene.h"
#include "../Stage/BlockManager.h"

class StageEditorScene : public BaseScene
{
public: //�萔
	static const int STAGE_WIDTH = 15;
	static const int STAGE_HEIGHT = 7;
	static const std::wstring resourcesDir;

public: //�ÓI�����o�ϐ�
	static BlockManager* block_mgr;
	static int wall_obj;
	static int door_obj;

public: //�����o�֐�
	StageEditorScene(DrawPolygon* draw, SceneChenger* sceneChenger);
	~StageEditorScene() override;

	void Init() override;
	void Update() override;
	void Draw() override;

private: //�����o�ϐ�
	int mapArray[STAGE_WIDTH * STAGE_HEIGHT];
	int mapIndex;
	int blockIndex;

	int background; //�w�i�摜
	int cursor; //�J�[�\���摜
};
