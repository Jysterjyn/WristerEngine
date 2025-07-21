#pragma once
#include <BaseScene.h>
#include <Ground.h>
#include <Player.h>

class TestSceneUIDrawer :public WristerEngine::_2D::AbstractUIDrawer
{
	// AbstractUIDrawer を介して継承されました
	void Initialize() override;
	void Update() override;
};

class TestScene : public WristerEngine::BaseScene
{
	std::vector<std::unique_ptr<Ground>> grounds;
	Player player;
	WE::_3D::Transform transform;

	// BaseScene を介して継承されました
	void Initialize() override;
	void Update() override;
	void Draw() {}
};