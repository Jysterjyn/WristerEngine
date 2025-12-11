#pragma once
#include <BaseScene.h>

class GameSceneUIDrawer : public WristerEngine::_2D::AbstractUIDrawer
{
	// AbstractUIDrawer を介して継承されました
	void Initialize() override;
	void Update() override;
};

class GameScene : public WristerEngine::BaseScene
{
	// BaseScene を介して継承されました
	void Initialize() override;
	void Update() override;
	void Draw() override;
};