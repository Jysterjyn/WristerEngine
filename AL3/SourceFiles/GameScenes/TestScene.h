#pragma once
#include <BaseScene.h>
#include <Player.h>

class TestSceneUIDrawer :public WristerEngine::_2D::AbstractUIDrawer
{
	// AbstractUIDrawer ����Čp������܂���
	void Initialize() override;
	void Update() override;
	void CreateNewSprite();
};

class TestScene : public WristerEngine::BaseScene
{
	std::unique_ptr<Player> player;
	WristerEngine::_3D::Camera* camera;
	bool isDebugCameraActive = false;

	// BaseScene ����Čp������܂���
	void Initialize() override;
	void Update() override;
};