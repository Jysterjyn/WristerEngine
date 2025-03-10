#pragma once
#include <ModelManager.h>
#include <Input.h>

class Player
{
	WristerEngine::_3D::Object3d* obj = nullptr;
	WristerEngine::Input* input = WristerEngine::Input::GetInstance();

public:
	void Initialize();
	void Update();
	void Draw();
};
