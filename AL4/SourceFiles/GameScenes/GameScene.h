#pragma once
#include <BaseScene.h>
#include <Player.h>
#include <Skydome.h>
#include <Ground.h>
#include <Enemy.h>
#include <CameraManager.h>
#include <list>
#include <LockOn.h>
#include <AudioManager.h>

class TestSceneUIDrawer :public WristerEngine::_2D::AbstractUIDrawer
{
	// AbstractUIDrawer ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	void Initialize() override {}
	void Update() override {}
};

class GameScene : public WristerEngine::BaseScene
{
	std::unique_ptr<Player> player;
	std::list<std::unique_ptr<WE::_3D::LockOnObject>> enemies;
	WE::Skydome skydome;
	Ground ground;
	WE::_3D::CameraManager* cMan = WE::_3D::CameraManager::GetInstance();
	std::unique_ptr<WE::_3D::LockOn> lockOn;
	bool inScreen = false;
	int32_t left = 0, right = 0, top = 0, bottom = 0;
	WE::PointAudio* audio = nullptr;

	// BaseScene ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	void Initialize() override;
	void Update() override;
	void Draw() { lockOn->Draw(); }
};