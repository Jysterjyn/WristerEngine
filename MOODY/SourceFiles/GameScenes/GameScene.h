#pragma once
#include <BaseScene.h>
#include <GameObjects.h>

class GameSceneUIDrawer : public WristerEngine::_2D::AbstractUIDrawer
{
	// AbstractUIDrawer を介して継承されました
	void Initialize() override;
	void Update() override;
};

class GameScene : public WristerEngine::BaseScene
{
	WE::uList<GameObjects> gameObjects;

	// BaseScene を介して継承されました
	void Initialize() override;
	void Update() override;
	void Draw() override;
};