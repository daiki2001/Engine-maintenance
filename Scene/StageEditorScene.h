#pragma once
#include "./Header/BaseScene.h"
#include "../Stage/BlockManager.h"

class StageEditorScene : public BaseScene
{
public: //定数
	static const int STAGE_WIDTH = 15;
	static const int STAGE_HEIGHT = 7;
	static const std::wstring resourcesDir;

public: //静的メンバ変数
	static BlockManager* block_mgr;
	static int wall_obj;
	static int door_obj;

public: //メンバ関数
	StageEditorScene(DrawPolygon* draw, SceneChenger* sceneChenger);
	~StageEditorScene() override;

	void Init() override;
	void Update() override;
	void Draw() override;

private: //メンバ変数
	int mapArray[STAGE_WIDTH * STAGE_HEIGHT];
	int mapIndex;
	int blockIndex;
	int background; //背景画像
};
