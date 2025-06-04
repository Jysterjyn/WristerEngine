#pragma once
#include <BaseScene.h>
#include <Player.h>
#include <skydome.h>
#include <Ground.h>
#include <Enemy.h>
#include <CameraManager.h>
#include <list>
#include <LockOn.h>

class TestSceneUIDrawer :public WristerEngine::_2D::AbstractUIDrawer
{
	// AbstractUIDrawer ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	void Initialize() override {}
	void Update() override {}
};

class GameScene : public WristerEngine::BaseScene
{
	std::unique_ptr<Player> player;
	std::list<std::unique_ptr<LockOnObject>> enemies;
	WE::Skydome skydome;
	Ground ground;
	WE::_3D::CameraManager* cMan = WE::_3D::CameraManager::GetInstance();
	std::unique_ptr<LockOn> lockOn;

	// BaseScene ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	void Initialize() override;
	void Update() override;
	void Draw() { lockOn->Draw(); }
};