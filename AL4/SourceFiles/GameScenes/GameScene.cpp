#include "GameScene.h"
#include "imgui.h"
#include <FollowCamera.h>
#include <DebugCamera.h>
#include <SceneManager.h>

void GameScene::Initialize()
{
	player = std::make_unique<Player>();
	player->Initialize("Player");

	enemy = std::make_unique<Enemy>();
	enemy->Initialize("Tomorou");

	WE::_3D::FollowCamera::Prop prop;
	prop.target = player->GetTransform();
	prop.posOffset = { 0.0f,2.0f,-12.0f };
	WE::_3D::DebugCamera::Prop dprop;
	dprop.targetPos = { 0,4,0 };
	dprop.distance = 10;
	dprop.mouseMoveDec = 200;
	dprop.wheelSpdDec = 100;
	cMan->Create("Debug", WE::_3D::CameraType::Debug, &dprop);
	cMan->Create("Player", WE::_3D::CameraType::Follow, &prop);
	player->SetCamera();

	skydome.Initialize("skydome", 2);
	ground.Initialize("cube", { 100,1,100 });
}

void GameScene::Update()
{
	if (input->IsTrigger(WE::Key::_1)) { cMan->Change("Debug"); }
	if (input->IsTrigger(WE::Key::_2)) { cMan->Change("Player"); }
	if (input->IsTrigger(WE::Key::_3)) { sceneManager->ChangeScene("GameScene"); }

	player->Update();
	enemy->Update();
}
