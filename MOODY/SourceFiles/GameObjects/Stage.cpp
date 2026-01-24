#include "Stage.h"
#include <Ball.h>
#include <Player.h>

void Stage::Initialize()
{
	WE::uPtr<Ball> ball = std::make_unique<Ball>();
	gameObjects.push_back(std::move(ball));

	WE::uPtr<Player> player = std::make_unique<Player>();
	gameObjects.push_back(std::move(player));

	for (auto& object : gameObjects) { object->Initialize(); }
}

void Stage::Update()
{
	gameObjects.remove_if([](const WE::uPtr<GameObjects>& o) { return o->IsDestroy(); });
	for (auto& object : gameObjects) { object->Update(); }
}

void Stage::Draw()
{
	for (auto& object : gameObjects) { object->Draw(); }
}
