#include "SceneFactory.h"
#include <GameScene.h>

WE::uPtr<WristerEngine::BaseScene> SceneFactory::CreateScene(const std::string& sceneName)
{
	WE::uPtr<WristerEngine::BaseScene> nextScene;

	if (sceneName == "GameScene") { nextScene = std::make_unique<GameScene>(); }
	assert(nextScene);
	nextScene->Initialize();
	return nextScene;
}
