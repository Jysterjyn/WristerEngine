#pragma once
#include "Object3d.h"
#include <Collider.h>

class BaseObject : public WE::Collider
{
protected:
	WE::_3D::ModelManager* mm = WE::_3D::ModelManager::GetInstance();
	WE::_3D::Object3d* object = nullptr;
};

class Ground : public BaseObject
{
public:
	void Initialize(const std::string& modelName, const Vector3& scale);
	void OnCollision() override { object->material.ambient = { 1,0,0 }; }
};

class Sphere : public BaseObject
{
public:
	void Initialize();
	void OnCollision() override { object->material.ambient = { 1,0,0 }; }
};