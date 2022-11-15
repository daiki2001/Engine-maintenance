#include "./Header/Scenemanager.h"
#include "./Header/Camera.h"
#include "./Header/DirectXInit.h"

#include "./Scene/EngineTestScene.h"
#include "../Scene/TitleScene.h"
#include "../Scene/GameScene.h"
#include "../Scene/SettingScene.h"
#include "../Scene/StageEditorScene.h"

#include "./Header/Error.h"

SceneManager::SceneManager(DrawPolygon* draw) :
	draw(draw)
{
	if (DirectXInit::EngineDebug)
	{
		std::shared_ptr<EngineTestScene> heep(new EngineTestScene(this->draw, this));
		sceneStack.push(std::move(heep));
	}
	else
	{
		sceneStack.push(std::make_shared<TitleScene>(this->draw, this));
	}
}

void SceneManager::Loop() const
{
	sceneStack.top()->Update();
	Camera::Update();
	sceneStack.top()->Draw();
}

void SceneManager::SceneChenge(SceneChenger::Scene scene, bool stackClear)
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
	case SceneChenger::Scene::Title:
		sceneStack.push(make_shared<TitleScene>(this->draw, this));
		break;
	case SceneChenger::Scene::Game:
		sceneStack.push(make_shared<GameScene>(this->draw, this));
		break;
	case SceneChenger::Scene::Setting:
		sceneStack.push(make_shared<SettingScene>(this->draw, this));
		break;
	case SceneChenger::Scene::StageEditer:
		sceneStack.push(make_shared<StageEditorScene>(this->draw, this));
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
