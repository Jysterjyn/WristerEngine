#pragma once
#include "AbstractSceneFactory.h"

class SceneFactory : public WristerEngine::AbstractSceneFactory
{
	// AbstractSceneFactory ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	std::unique_ptr<WristerEngine::BaseScene> CreateScene(const std::string& sceneName) override;
};