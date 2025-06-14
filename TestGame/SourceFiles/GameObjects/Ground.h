#pragma once
#include "Object3d.h"
#include <Collider.h>
#include <Input.h>

class BaseObject : public WE::Collider
{
protected:
	WE::_3D::ModelManager* mm = WE::_3D::ModelManager::GetInstance();
	WE::_3D::Object3d* object = nullptr;
	WE::Input* input = WE::Input::GetInstance();

public:
	virtual ~BaseObject() = default;
	virtual void Update() { object->material.ambient = { 1,1,1 }; }
};

class Ground : public BaseObject
{
public:
	void Initialize(const std::string& modelName, const Vector3& scale);
	void OnCollision() override 
	{
		object->material.ambient = { 1,0,0 }; 
	}
};

class Sphere : public BaseObject
{
public:
	void Initialize();
	void Update() override;
	void OnCollision() override { object->material.ambient = { 1,0,0 }; }
};