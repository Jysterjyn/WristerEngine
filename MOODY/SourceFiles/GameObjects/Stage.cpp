#include "Stage.h"
#include <Ball.h>
#include <Player.h>
#include <imgui.h>

void Stage::CreateBall()
{
	gameObjects.Add(std::make_unique<Ball>());
	WE::uPtr<WE::TimedCall> timedCall = std::make_unique<WE::TimedCall>([&]() { CreateBall(); }, 120);
	timedCalls.Add(std::move(timedCall));
}

void Stage::Initialize()
{
	//gameObjects.Add(std::make_unique<Ball>());
	CreateBall();
	gameObjects.Add(std::make_unique<Player>());
}

void Stage::Update()
{
	gameObjects.Update();
	timedCalls.Update();
}

void Stage::Draw()
{
	gameObjects.Draw();
}
