#pragma once
#include "ISceneFactory.h"

class SceneFactory : public WristerEngine::ISceneFactory
{
	// ISceneFactory を介して継承されました
	std::unique_ptr<WristerEngine::BaseScene> CreateScene(const std::string& sceneName) override;
};