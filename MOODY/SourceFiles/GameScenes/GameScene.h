#pragma once
#include <BaseScene.h>
#include <Stage.h>

class GameSceneUIDrawer : public WE::_2D::AbstractUIDrawer
{
	// AbstractUIDrawer を介して継承されました
	void Initialize() override;
	void Update() override;
};

class GameScene : public WE::BaseScene
{
	Stage stage;

	// BaseScene を介して継承されました
	void Initialize() override;
	void Update() override;
	void Draw() override;
};