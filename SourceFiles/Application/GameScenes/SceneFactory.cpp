#include "SceneFactory.h"
#include <TestScene.h>

std::unique_ptr<WristerEngine::BaseScene> SceneFactory::CreateScene(const std::string& sceneName)
{
	std::unique_ptr<WristerEngine::BaseScene> nextScene;

	if (sceneName == "TestScene") { nextScene = std::make_unique<TestScene>(); }
	assert(nextScene);
	return nextScene;
}
