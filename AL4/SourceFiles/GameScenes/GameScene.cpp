#include "GameScene.h"
#include "imgui.h"
#include <FollowCamera.h>

void TestScene::Initialize()
{
	player = std::make_unique<Player>();
	player->Initialize("Player");

	enemy = std::make_unique<Enemy>();
	enemy->Initialize("Tomorou");

	WE::_3D::FollowCamera::Prop prop;
	prop.target = player->GetTransform();
	prop.posOffset = { 0.0f,2.0f,-10.0f };
	WE::_3D::CameraManager::GetInstance()->Create("Player", WE::_3D::CameraType::Follow, &prop);
	player->SetCamera();

	skydome.Initialize("skydome", 2);
	ground.Initialize("cube", { 100,1,100 });
}

void TestScene::Update()
{
	player->Update();
	enemy->Update();
}
