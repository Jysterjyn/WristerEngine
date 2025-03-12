#pragma once
#include <BaseScene.h>
#include <Player.h>
#include <Enemy.h>

class TestSceneUIDrawer :public WristerEngine::_2D::AbstractUIDrawer
{
	// AbstractUIDrawer ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	void Initialize() override;
	void Update() override;
};

class TestScene : public WristerEngine::BaseScene
{
	std::unique_ptr<Player> player;
	WristerEngine::_3D::Camera* camera;
	bool isDebugCameraActive = false;
	std::unique_ptr<Enemy> enemy;
	// BaseScene ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	void Initialize() override;
	void Update() override;
};