#include "EngineTestScene.h"
#include "./Header/DirectXInit.h"
#include "./ShaderMgr/ShaderManager.h"
#include "./Header/Input.h"
#include "./Header/Camera.h"

namespace
{
ShaderManager* shaderMgr = ShaderManager::Get();
DirectX::XMFLOAT4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
}

EngineTestScene::EngineTestScene(DrawPolygon* draw, SceneChenger* sceneChenger) :
	BaseScene(draw, sceneChenger),
	fbxLoader(FbxLoader::GetInstance()),
	background(FUNCTION_ERROR),
	fbxModel(FUNCTION_ERROR),
	fbxModel2(FUNCTION_ERROR),
	postEffect{},
	useShader(0),
	defaultPostEffect(FUNCTION_ERROR)
{
	postEffect.Init();
	Init();
}

EngineTestScene::~EngineTestScene()
{
	fbxLoader->Finalize();
}

void EngineTestScene::Init()
{
	using namespace DirectX;
	using namespace Math;

	fbxLoader->Init();
	fbxModel = fbxLoader->LoadModelFromFile(resourcesDirectory + "boneTest.fbx");
	fbxModel2 = fbxLoader->LoadModelFromFile(resourcesDirectory + "cube/cube.fbx");
	fbxLoader->GetModel(fbxModel)->Init();
	fbxLoader->GetModel(fbxModel2)->Init();

	// 画像の読み込み
	background = draw->LoadTextrue(StringToWString(resourcesDirectory + "background.png").c_str());

	Camera::targetRadius = 10.0f;
	Camera::longitude = DEGREE_F * (0.0f);
	Camera::latitude = DEGREE_F * (0.0f);

	Camera::pos = { 0.0f, 0.0f, -10.0f };
	Camera::target = { 0.0f, 2.5f, 0.0f };
	Camera::upVec = { 0.0f, 1.0f, 0.0f };

	fbxLoader->GetModel(fbxModel)->pos = { 0.0f, 0.0f, 0.0f };
	fbxLoader->GetModel(fbxModel)->PlayAnimation();
	fbxLoader->GetModel(fbxModel2)->pos = { -100.0f, -10.0f, -50.0f };

	int shader = shaderMgr->CreateShader(shaderMgr->GetShader(postEffect.GetShader()).GetVertex(),
										 StringToWString(resourcesDirectory + "defaultPS.hlsl").c_str());
	int gPipeline = shaderMgr->CreateGPipeline(shader, postEffect.GetInputLayout());
	for (size_t i = 0; i < 5; i++)
	{
		shaderMgr->GetGraphicsPipeline(gPipeline, static_cast<ShaderManager::BlendMode>(i)).pRootSignature =
			postEffect.GetRootSignature();
	}
	defaultPostEffect = shaderMgr->CreatePipelineState(gPipeline);
}

void EngineTestScene::Update()
{
	using namespace DirectX;
	using namespace Math;
	using namespace Collision;

	if (Input::IsKey(DIK_R))
	{
		Camera::targetRadius = 20.0f;
		Camera::longitude = DEGREE_F * (0.0f);
		Camera::latitude = DEGREE_F * (0.0f);
	}

	if (Input::IsKey(DIK_A))
	{
		Camera::longitude -= DEGREE_F;
		if (Camera::longitude <= DEGREE_F * 0.0)
		{
			Camera::longitude += DEGREE_F * 360;
		}
	}
	if (Input::IsKey(DIK_D))
	{
		Camera::longitude += DEGREE_F;
		if (Camera::longitude >= DEGREE_F * 360.0)
		{
			Camera::longitude -= DEGREE_F * 360;
		}
	}
	if (Input::IsKey(DIK_W))
	{
		Camera::latitude += DEGREE_F;
		if (Camera::latitude >= DEGREE_F * 90.0f)
		{
			Camera::latitude = DEGREE_F * 90.0f - DEGREE_F;
		}
	}
	if (Input::IsKey(DIK_S))
	{
		Camera::latitude -= DEGREE_F;
		if (Camera::latitude <= DEGREE_F * -90.0f)
		{
			Camera::latitude = -(DEGREE_F * 90.0f - DEGREE_F);
		}
	}

	if (Input::IsKey(DIK_PGUP))
	{
		Camera::targetRadius -= 1.0f;
		if (Camera::targetRadius < 10.0f)
		{
			Camera::targetRadius = 10.0f;
		}
	}
	if (Input::IsKey(DIK_PGDN))
	{
		Camera::targetRadius += 1.0f;
	}

	if (Input::IsKey(DIK_UP))
	{
		color.x += 0.01f;
		if (color.x >= 1.0f)
		{
			color.x = 1.0f;
		}
	}
	if (Input::IsKey(DIK_DOWN))
	{
		color.x -= 0.01f;
		if (color.x <= 0.0f)
		{
			color.x = 0.0f;
		}
	}

	if (Input::IsKeyTrigger(DIK_RETURN))
	{
		useShader++;
		if (useShader > 2)
		{
			useShader = 0;
		}
	}

	Camera::pos.x = cosf(Camera::longitude) * cosf(Camera::latitude);
	Camera::pos.y = sinf(Camera::latitude);
	Camera::pos.z = sinf(Camera::longitude) * cosf(Camera::latitude);
	Camera::pos *= Camera::targetRadius;
	Camera::pos += Camera::target;
	Camera::SetCamera(Camera::pos, Camera::target, Camera::upVec);

	fbxLoader->GetModel(fbxModel)->Update(color);
	fbxLoader->GetModel(fbxModel2)->Update(color);
}

void EngineTestScene::Draw()
{
	using namespace DirectX;
	using namespace Math;

	DirectXInit* w = DirectXInit::GetInstance();

	postEffect.PreDraw();
	draw->SetDrawBlendMode(DirectDrawing::BlendMode::ALPHA);

	if (useShader < 2)
	{
		switch (useShader)
		{
		case 0:
			Model::ChangeDefaultShader();
			break;
		case 1:
			Model::ChangeColorShader();
			break;
		default:
			break;
		}
	}
	else
	{
		Model::ChangeDefaultShader();
	}
	fbxLoader->GetModel(fbxModel)->Draw();
	fbxLoader->GetModel(fbxModel2)->Draw();

	postEffect.PostDraw();

	w->ClearScreen();

	shaderMgr->ChangePipelineState(
		DirectXInit::GetCommandList(),
		DirectDrawing::GetSpriteRootSignature(),
		DirectDrawing::GetSpritePipelineState());
	// 背景
	draw->DrawTextrue(
		0.0f,
		0.0f,
		static_cast<float>(w->windowWidth),
		static_cast<float>(w->windowHeight),
		0.0f,
		background,
		XMFLOAT2(0.0f, 0.0f)
	);

	if (useShader < 2)
	{
		shaderMgr->ChangePipelineState(
			DirectXInit::GetCommandList(),
			postEffect.GetRootSignature(),
			defaultPostEffect);
	}
	else
	{
		switch (useShader)
		{
		case 2:
			shaderMgr->ChangePipelineState(
				DirectXInit::GetCommandList(),
				postEffect.GetRootSignature(),
				postEffect.GetPipelineState());
			break;
		default:
			break;
		}
	}
	postEffect.Draw();

	shaderMgr->ChangePipelineState(
		DirectXInit::GetCommandList(),
		DirectDrawing::GetSpriteRootSignature(),
		DirectDrawing::GetSpritePipelineState());
	// 前景
	draw->DrawString(
		10.0f,
		10.0f,
		2.0f,
		color,
		"%d",
		useShader
	);
	w->ScreenFlip();

	// ループの終了処理
	draw->PolygonLoopEnd();
}
