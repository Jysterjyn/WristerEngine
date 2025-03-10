#pragma once
#include <ModelManager.h>

class Player
{
	WristerEngine::_3D::Object3d* obj = nullptr;

public:
	void Initialize();
	void Update();
	void Draw();
};
