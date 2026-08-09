#include "GameScene.h"

void GameSceneUIDrawer::Initialize()
{
	sprites["bg"] = spMan->Create({ "illust_66697251_20260725_195447.jpg" });
	float scale = max(WE::WIN_SIZE.x / sprites["bg"]->size.x, WE::WIN_SIZE.y / sprites["bg"]->size.y);
	sprites["bg"]->size *= scale;
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
}

void GameScene::Draw()
{
	uiDrawer->Draw();
	stage.Draw();
}
