#pragma once
#include <ModelManager.h>
#include <unordered_map>

class Player
{
	std::unordered_map<std::string, WE::_3D::Object3d*> objects;
	WE::_3D::Transform rootPos;

public:
	void Initialize();
	void Update();
	void Draw();
};
