#include "GameScene.h"

void GameSceneUIDrawer::Initialize()
{
	sprites["bg"] = spMan->Create({ "Background.png" });
	sprites["bg"]->size = { 1280,720 };
	sprites["bg"]->color = { 0.5f,0.5f,0.5f };
}

void GameSceneUIDrawer::Update()
{
}

void GameScene::Initialize()
{
	uiDrawer = std::make_unique<GameSceneUIDrawer>();
	uiDrawer->Initialize();

	stage.Initialize();
}

void GameScene::Update()
{
	uiDrawer->Update();

	stage.Update();
	collision->CheckCircleCollisions();
}

void GameScene::Draw()
{
	uiDrawer->Draw();
	stage.Draw();
}
