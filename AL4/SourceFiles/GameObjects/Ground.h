#pragma once
#include "Object3d.h"

class Ground
{
private:
	WE::_3D::Object3d* object = nullptr;

public:
	void Initialize(const std::string& modelName, const Vector3& scale);
};

