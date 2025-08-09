#pragma once
#include <BaseScene.h>
#include <Ground.h>
#include <Player.h>
#include <Behavior.h>

struct BehaviorParameter : WE::Behavior::Parameter
{
	Vector2 pos;
};

class RightMove : public WE::Behavior
{
	BehaviorParameter* pParam = nullptr;
	WE::FrameTimer timer = 300;
	// Behavior を介して継承されました
	void Initialize() override;
	void Update() override;
};

class UpMove : public WE::Behavior
{
	BehaviorParameter* pParam = nullptr;
	WE::FrameTimer timer = 300;
	// Behavior を介して継承されました
	void Initialize() override;
	void Update() override;
};

class TestSceneUIDrawer : public WristerEngine::_2D::AbstractUIDrawer
{
	WE::_2D::Sprite* sprite = nullptr;
	WE::BehaviorPattern behaviorPattern;
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
	//void Draw() {}
};