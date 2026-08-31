#pragma once
#include <GameObjects.h>

class Stage
{
	WE::uList2 gameObjects;

public:
	void Initialize();
	void Update();
	void Draw();
	~Stage() { gameObjects.clear(); }
};