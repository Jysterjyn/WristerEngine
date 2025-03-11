#pragma once
#include <BaseScene.h>
#include <Player.h>
#include <Enemy.h>

class TestSceneUIDrawer :public WristerEngine::_2D::AbstractUIDrawer
{
	// AbstractUIDrawer ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	void Initialize() override;
	void Update() override;
	void CreateNewSprite();
};

class TestScene : public WristerEngine::BaseScene
{
	std::unique_ptr<Player> player;
	WristerEngine::_3D::Camera* camera;
	bool isDebugCameraActive = false;
	std::unique_ptr<Enemy> enemy;
	std::array<WristerEngine::_3D::Object3d*, 3> obj{};
	WristerEngine::_3D::Object3d* sphere = nullptr;
	float t = 0;
	const Vector3 v0 = { 3,8,-4 };
	const Vector3 v1 = { -1,-6,2 };

	// BaseScene ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	void Initialize() override;
	void Update() override;
};