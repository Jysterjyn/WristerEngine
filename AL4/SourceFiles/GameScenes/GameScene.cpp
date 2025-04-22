#include "GameScene.h"
#include "imgui.h"

void TestScene::Initialize()
{
	player = std::make_unique<Player>();
	player->Initialize();
}

void TestScene::Update()
{
	player->Update();
}
