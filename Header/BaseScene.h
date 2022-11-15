#pragma once
#include "DrawPolygon.h"
#include "SceneChenge.h"

class BaseScene
{
public:
	BaseScene() = delete;
	BaseScene(DrawPolygon* draw, SceneChenger* sceneChenger);
	virtual ~BaseScene();

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

protected:
	DrawPolygon* draw;
	SceneChenger* sceneChenger;
};
