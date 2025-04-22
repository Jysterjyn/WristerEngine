#include "GameScene.h"
#include "ModelManager.h"
#include "imgui.h"

void TestScene::Initialize()
{
	//debugCamera;

	//WristerEngine::_3D::ModelManager::SetCamera(&debugCamera);
	player = std::make_unique<Player>();
	player->Initialize();
}

void TestScene::Update()
{
	player->Update();
}
