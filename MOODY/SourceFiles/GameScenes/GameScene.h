#pragma once
#include <BaseScene.h>
#include <Stage.h>
#include <Collider.h>

class GameSceneUIDrawer : public WristerEngine::_2D::AbstractUIDrawer
{
	// AbstractUIDrawer を介して継承されました
	void Initialize() override;
	void Update() override;
};

class GameScene : public WristerEngine::BaseScene
{
	Stage stage;
	WE::_2D::TestCheckAllCircleCollision* collision = WE::_2D::TestCheckAllCircleCollision::GetInstance();

	// BaseScene を介して継承されました
	void Initialize() override;
	void Update() override;
	void Draw() override;

public:
	~GameScene() 
	{
		collision->Clear(); 
	}
};