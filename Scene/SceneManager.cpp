#include "SceneManager.h"
#include "./Header/Camera.h"
#include "./Header/DirectXInit.h"

#include "EngineTestScene.h"
#include "../Scene/TitleScene.h"
#include "../Scene/GameScene.h"
#include "../Scene/SettingScene.h"
#include "../Scene/StageEditorScene.h"
#include "../Scene/StageSelectScene.h"

#include "./Header/Error.h"

SceneManager::SceneManager(DrawPolygon* draw) :
	draw(draw)
{
	BaseScene::StaticInit(this->draw);

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
	sceneStack.top()->Update();
	BaseScene::ChangeAnimationUpdate();
	Camera::Update();

	static auto w = DirectXInit::GetInstance();
	w->ClearScreen();
	sceneStack.top()->Draw();
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
