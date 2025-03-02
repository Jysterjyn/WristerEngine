#pragma once
#include "AbstractSceneFactory.h"

class SceneFactory : public WristerEngine::AbstractSceneFactory
{
	// AbstractSceneFactory ����Čp������܂���
	std::unique_ptr<WristerEngine::BaseScene> CreateScene(const std::string& sceneName) override;
};