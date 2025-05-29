#pragma once
#include <BaseScene.h>
#include <Player.h>
#include <skydome.h>
#include <Ground.h>
#include <Enemy.h>
#include <CameraManager.h>

class TestSceneUIDrawer :public WristerEngine::_2D::AbstractUIDrawer
{
	// AbstractUIDrawer ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	void Initialize() override {}
	void Update() override {}
};

class GameScene : public WristerEngine::BaseScene
{
	std::unique_ptr<Player> player;
	std::unique_ptr<Enemy> enemy;
	WE::Skydome skydome;
	Ground ground;
	WE::_3D::CameraManager* cMan = WE::_3D::CameraManager::GetInstance();

	// BaseScene ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	void Initialize() override;
	void Update() override;
};