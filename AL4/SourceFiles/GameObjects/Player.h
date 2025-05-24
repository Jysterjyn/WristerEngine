#pragma once
#include <ModelManager.h>
#include <unordered_map>
#include <Input.h>

class Player
{
	std::unordered_map<std::string, WE::_3D::Object3d*> objects;
	WE::_3D::Transform rootPos;
	WE::Input* input = WE::Input::GetInstance();

	void Move();

public:
	void Initialize();
	void Update();
	void Draw();
	const WE::_3D::Transform* GetTransform() const { return &rootPos; }
};
