#pragma once
#include "ISceneFactory.h"

class SceneFactory : public WE::ISceneFactory
{
	// ISceneFactory を介して継承されました
	WE::uPtr<WE::BaseScene> CreateScene(WE::CR<std::string> sceneName) override;
};