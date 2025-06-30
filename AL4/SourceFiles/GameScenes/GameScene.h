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
	// AbstractUIDrawer を介して継承されました
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

	// BaseScene を介して継承されました
	void Initialize() override;
	void Update() override;
	void Draw() { lockOn->Draw(); }
};