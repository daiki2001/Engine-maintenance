#include "SceneChangeAnimation.h"
#include "./Header/DirectXInit.h"
#include "./Header/EngineGeneral.h"
#include "./Header/Parameter.h"

namespace
{
constexpr int maxSize = 1300;
}

DrawPolygon* SceneChangeAnimation::draw = nullptr;

SceneChangeAnimation::SceneChangeAnimation(DrawPolygon* draw) :
	isAnimation(false),
	isChange(false),
	radius(1),
	maskScreen(-1),
	tempScreen(-1),
	circle(FUNCTION_ERROR),
	sound(FUNCTION_ERROR)
{
	if (draw == nullptr) return;

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

	static const int changeSpeed = 100;

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
	static const int winW = DirectXInit::GetInstance()->windowWidth;
	static const int winH = DirectXInit::GetInstance()->windowHeight;
	// マスク用の画面に円を描画
	//SetDrawScreen(maskScreen);
	//ClearDrawScreen();

	if (isAnimation)
	{
		if (radius > 0)
		{
			//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0x80);
			//DrawCircle(WIN_WIDTH / 2, WIN_HEIGHT / 2, radius, color, true);
			//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0xFF);
			//DrawCircle(WIN_WIDTH / 2, WIN_HEIGHT / 2, radius - 128, color, true);
			draw->ChangeSpriteShader();
			draw->DrawTextrue(winW / 2.0f,
							  winH / 2.0f,
							  static_cast<float>(radius),
							  static_cast<float>(radius),
							  0.0f,
							  Parameter::Get("white1x1"),
							  DirectX::XMFLOAT2(0.5f, 0.5f),
							  DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
		}
	}
	else
	{
		//DrawBox(0, 0, WIN_WIDTH, WIN_HEIGHT, color, true);
	}

	// 裏画面に描画先を変更
	//SetDrawScreen(DX_SCREEN_BACK);
	//ClearDrawScreen();

	// 描画時の合成画像にマスク画面を設定
	//SetBlendGraphParam(maskScreen, DX_BLENDGRAPHTYPE_ALPHA);

	if (isAnimation)
	{
		//DrawBox(0, 0, WIN_WIDTH, WIN_HEIGHT, color, true);
	}

	// 裏画面に一時保存用のスクリーンを描画
	//DrawGraph(0, 0, tempScreen, true);

	// 合成設定を解除
	//SetBlendGraphParam(-1, DX_BLENDGRAPHTYPE_NORMAL);
	// 一時保存用のスクリーンに描画先を変更
	//SetDrawScreen(tempScreen);
}

void SceneChangeAnimation::Start()
{
	isAnimation = true;
	isChange = false;
	radius = 1;
	//PlaySoundMem(sound, DX_PLAYTYPE_BACK);
}

void SceneChangeAnimation::Load()
{
	ScreenInit();

	if (circle == FUNCTION_ERROR)
	{
		//circle = draw->CreateCircle(1.0f, 16);
	}
	if (sound == FUNCTION_ERROR)
	{
		//sound = LoadSoundMem("./Resources/sound/SE/changescene.mp3");
		//ChangeVolumeSoundMem(0x80, sound);
	}
}

void SceneChangeAnimation::Release()
{
	//if (DeleteGraph(maskScreen) == 0)
	//{
	//	maskScreen = -1;
	//}
	//if (DeleteGraph(tempScreen) == 0)
	//{
	//	tempScreen = -1;
	//}
	//if (DeleteSoundMem(sound) == 0)
	//{
	//	sound = FUNCTION_ERROR;
	//}
}

void SceneChangeAnimation::ScreenInit()
{
	if (maskScreen == -1)
	{
		//maskScreen = MakeScreen(General::WIN_WIDTH, General::WIN_HEIGHT, true);
	}
	if (tempScreen == -1)
	{
		//tempScreen = MakeScreen(General::WIN_WIDTH, General::WIN_HEIGHT, true);
		// 一時保存用のスクリーンに描画先を変更
		//SetDrawScreen(tempScreen);
	}
}
