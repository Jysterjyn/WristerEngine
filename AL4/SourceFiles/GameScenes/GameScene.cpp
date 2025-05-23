#include "GameScene.h"
#include "imgui.h"

void TestScene::Initialize()
{
	player = std::make_unique<Player>();
	player->Initialize();

	skydome.Initialize("skydome", 2);
	WE::_3D::CameraManager::GetInstance()->Create("debug", WE::_3D::CameraType::Debug);

	ground.Initialize("cube", { 100,1,100 });
}

void TestScene::Update()
{
	player->Update();
}
