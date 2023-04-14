#include "SceneChangeAnimation.h"
#include "./Header/DirectXInit.h"
#include "./Header/DrawPolygon.h"
#include "./Header/EngineGeneral.h"
#include "./Header/Parameter.h"

SceneChangeAnimation::SceneChangeAnimation() :
	isAnimation(false),
	isChange(false),
	radius(1),
	maskScreen(-1),
	tempScreen(-1),
	circle(FUNCTION_ERROR),
	sound(FUNCTION_ERROR)
{
	Init();
}

SceneChangeAnimation::~SceneChangeAnimation()
{
	Release();
}

void SceneChangeAnimation::Init()
{
	isAnimation = false;
	isChange = false;
	radius = 1;

	Load();
}

void SceneChangeAnimation::Update()
{
	if (isAnimation == false)
	{
		return;
	}

	static const int maxSize = DirectXInit::GetInstance()->windowWidth + 150;
	static const int changeSpeed = 50;

	if (isChange)
	{
		radius -= changeSpeed;
		if (radius < 0)
		{
			isAnimation = false;
		}
	}
	else
	{
		radius += changeSpeed;
		if (radius > maxSize)
		{
			isChange = true;
		}
	}
}

void SceneChangeAnimation::Draw()
{
	auto w = DirectXInit::GetInstance();
	const int winW = w->windowWidth;
	const int winH = w->windowHeight;
	auto draw = Library::DrawPolygon::GetInstance();

	if (isAnimation)
	{
		if (radius > 0)
		{
			draw->ChangeSpriteShader();
			draw->DrawTextrue(winW / 2.0f,
							  winH / 2.0f,
							  static_cast<float>(radius),
							  static_cast<float>(radius),
							  0.0f,
							  Parameter::Get("white1x1"),
							  DirectX::XMFLOAT2(0.5f, 0.5f),
							  Color::AddAlphaValue(Color::BLACK, 1.0f));
		}
	}
}

void SceneChangeAnimation::Start()
{
	isAnimation = true;
	isChange = false;
	radius = 1;
}

void SceneChangeAnimation::Load()
{
	ScreenInit();
}

void SceneChangeAnimation::Release()
{
}

void SceneChangeAnimation::ScreenInit()
{
}
