#include "SceneFactory.h"
#include <GameScene.h>

WE::uPtr<WE::BaseScene> SceneFactory::CreateScene(WE::CR<std::string> sceneName)
{
	WE::uPtr<WE::BaseScene> nextScene;

	if (sceneName == "GameScene") { nextScene = std::make_unique<GameScene>(); }
	assert(nextScene);
	nextScene->Initialize();
	return nextScene;
}
