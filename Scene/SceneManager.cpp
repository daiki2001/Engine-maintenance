#include "SceneManager.h"
#include "./Header/DirectXInit.h"
#include "./Header/DrawPolygon.h"
#include "./Header/Camera.h"
#include "./Input/Input.h"

#include "EngineTestScene.h"
#include "../Scene/TitleScene.h"
#include "../Scene/GameScene.h"
#include "../Scene/SettingScene.h"
#include "../Scene/StageEditorScene.h"
#include "../Scene/StageSelectScene.h"

#include "./Header/Parameter.h"
#include "../UI.h"
#include "./Header/Error.h"

PostEffect SceneManager::postEffect;

SceneManager::SceneManager()
{
	UI::StaticInit();
	postEffect.Init();

	if (DirectXInit::EngineDebug)
	{
		std::shared_ptr<EngineTestScene> heep(new EngineTestScene(this));
		sceneStack.push(std::move(heep));
	}
	else
	{
		sceneStack.push(std::make_shared<TitleScene>(this));
	}
}

void SceneManager::Loop() const
{
	static auto w = DirectXInit::GetInstance();
	static auto draw = Library::DrawPolygon::GetInstance();

	sceneStack.top()->Update();
	sceneStack.top()->ChengeTitleScene(Input::IsKeyTrigger(DIK_F1));
	BaseScene::ChangeAnimationUpdate();
	Camera::Update();

	postEffect.PreDraw();
	draw->SetDrawBlendMode(ShaderManager::BlendMode::ALPHA);
	sceneStack.top()->Draw();
	postEffect.PostDraw();

	w->ClearScreen();
	// 背景
	sceneStack.top()->BGDraw();
	// オブジェクト
	postEffect.Draw();
	// 前景
	sceneStack.top()->UIDraw();
	BaseScene::ChangeAnimationDraw();
	w->ScreenFlip();

	// ループの終了処理
	draw->PolygonLoopEnd();
}

void SceneManager::SceneChange(SceneChanger::Scene scene, bool stackClear)
{
	using namespace std;

	if (stackClear)
	{
		while (sceneStack.empty() == false)
		{
			sceneStack.pop();
		}
	}

	switch (scene)
	{
	case SceneChanger::Scene::Title:
		sceneStack.push(make_shared<TitleScene>(this));
		break;
	case SceneChanger::Scene::Game:
		sceneStack.push(make_shared<GameScene>(this));
		break;
	case SceneChanger::Scene::Setting:
		sceneStack.push(make_shared<SettingScene>(this));
		break;
	case SceneChanger::Scene::StageEditer:
		sceneStack.push(make_shared<StageEditorScene>(this));
		break;
	case SceneChanger::Scene::StageSelect:
		sceneStack.push(make_shared<StageSelectScene>(this));
		break;
	default:
		Engine::ErrorLog("存在しないシーンが呼ばれました\n", true);
		break;
	}
}

int SceneManager::PopScene()
{
	if (sceneStack.size() <= 1)
	{
		return Engine::FUNCTION_ERROR;
	}

	sceneStack.pop();

	return 0;
}
