#pragma once
#include <BaseScene.h>
#include <Player.h>
#include <Enemy.h>
#include <RailCamera.h>
#include <Skydome.h>
#include <PrimitiveDrawer.h>
#include <FbxLoader.h>

class GamePlaySceneUIDrawer :public WristerEngine::_2D::AbstractUIDrawer
{
	// AbstractUIDrawer ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
	void Initialize() override;
	void Update() override;
};

class GamePlayScene : public WristerEngine::BaseScene
{
	std::unique_ptr<Player> player;
	WristerEngine::_3D::BaseCamera* camera;
	bool isDebugCameraActive = false;
	std::unique_ptr<Enemy> enemy;
	std::unique_ptr<WristerEngine::_3D::RailCamera> railCamera;
	std::unique_ptr<WristerEngine::Skydome> skydome;
	WristerEngine::_3D::PrimitiveDrawer* primitiveDrawer = WristerEngine::_3D::PrimitiveDrawer::GetInstance();

	// BaseScene ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
	void Initialize() override;
	void Update() override;
	void Draw() override { WristerEngine::BaseScene::Draw(); player->Draw(); }
};