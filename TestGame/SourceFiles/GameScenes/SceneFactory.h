#pragma once
#include "AbstractSceneFactory.h"

class SceneFactory : public WristerEngine::AbstractSceneFactory
{
	// AbstractSceneFactory を介して継承されました
	std::unique_ptr<WristerEngine::BaseScene> CreateScene(const std::string& sceneName) override;
};