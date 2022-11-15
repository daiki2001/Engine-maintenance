﻿#include "BaseScene.h"
#include "./Header/DirectXInit.h"

DrawPolygon* BaseScene::draw = nullptr;
SceneChangeAnimation BaseScene::changeAnimation = nullptr;

BaseScene::BaseScene(SceneChanger* sceneChanger) :
	sceneChanger(sceneChanger),
	isSceneDest(false),
	nextScene(SceneChanger::Scene::Title)
{
	changeAnimation = BaseScene::draw;
}

BaseScene::~BaseScene()
{
	sceneChanger = nullptr;
}
