#include "SceneFactory.h"
#include <GameScene.h>

std::unique_ptr<WristerEngine::BaseScene> SceneFactory::CreateScene(const std::string& sceneName)
{
	std::unique_ptr<WristerEngine::BaseScene> nextScene;

	if (sceneName == "GameScene") { nextScene = std::make_unique<GameScene>(); }
	assert(nextScene);
	return nextScene;
}
