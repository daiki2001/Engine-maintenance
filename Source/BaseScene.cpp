#include "./Header/BaseScene.h"
#include "./Header/DirectXInit.h"

BaseScene::BaseScene(DrawPolygon* draw, SceneChenger* sceneChenger) :
	draw(draw),
	sceneChenger(sceneChenger)
{
}

BaseScene::~BaseScene()
{
	draw = nullptr;
	sceneChenger = nullptr;
}
