#pragma once
#include <GameObjects.h>
#include <TimedCall.h>

class Stage
{
	WE::uList gameObjects;

	void CreateBall();

	WE::uList timedCalls;

public:
	void Initialize();
	void Update();
	void Draw();
	~Stage() { gameObjects.clear(); }
};