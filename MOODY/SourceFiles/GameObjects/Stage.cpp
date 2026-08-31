#include "Stage.h"
#include <Ball.h>
#include <Player.h>
#include <imgui.h>

void Stage::Initialize()
{
	gameObjects.Add(std::make_unique<Ball>());
	gameObjects.Add(std::make_unique<Player>());
}

void Stage::Update()
{
	gameObjects.Update();
}

void Stage::Draw()
{
	gameObjects.Draw();
}
