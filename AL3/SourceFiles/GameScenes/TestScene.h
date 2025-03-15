#pragma once
#include <BaseScene.h>
#include <Player.h>
#include <Enemy.h>
#include <RailCamera.h>

class TestSceneUIDrawer :public WristerEngine::_2D::AbstractUIDrawer
{
	// AbstractUIDrawer ����Čp������܂���
	void Initialize() override;
	void Update() override;
};

class TestScene : public WristerEngine::BaseScene
{
	std::unique_ptr<Player> player;
	WristerEngine::_3D::Camera* camera;
	bool isDebugCameraActive = false;
	std::unique_ptr<Enemy> enemy;
	WristerEngine::_3D::RailCamera railCamera;
	
	// BaseScene ����Čp������܂���
	void Initialize() override;
	void Update() override;
};