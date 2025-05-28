#pragma once
#include <Object3d.h>
#include <unordered_map>

class BaseCharacter
{
protected:
	std::unordered_map<std::string, WE::_3D::Object3d*> objects;
	WE::_3D::Transform rootPos;

public:
	virtual void Initialize(const std::string& modelGroupName);
	virtual void Update() { rootPos.Update(); }
	virtual void Draw() {}
	const WE::_3D::Transform* GetTransform() const { return &rootPos; }
};

