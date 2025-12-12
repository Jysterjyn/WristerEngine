#include "GameScene.h"
#include <Ball.h>

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

	WE::uPtr<Ball> ball = std::make_unique<Ball>();
	gameObjects.push_back(std::move(ball));

	for (auto& object : gameObjects) { object->Initialize(); }
}

void GameScene::Update()
{
	uiDrawer->Update();

	gameObjects.remove_if([](const WE::uPtr<GameObjects>& o) { return o->IsDestroy(); });
	for (auto& object : gameObjects) { object->Update(); }
}

void GameScene::Draw()
{
	uiDrawer->Draw();
	for (auto& object : gameObjects) { object->Draw(); }
}
