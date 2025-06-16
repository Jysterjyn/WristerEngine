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
	static uint32_t inputIndex;

public:
	virtual ~BaseObject() = default;
	virtual void Update() { object->material.ambient = { 1,1,1 }; }
	static void ChangeIndex() { inputIndex = (inputIndex + 1) % 2; }
};

class Ground : public BaseObject
{
	WE::PlaneCollider* collider = nullptr;

public:
	void Initialize(const std::string& modelName, const Vector3& scale);
	void Update() override;
	void OnCollision() override;
};

class Sphere : public BaseObject
{
	WE::SphereCollider* collider = nullptr;

public:
	void Initialize();
	void Update() override;
	void OnCollision() override;
};

class Triangle : public BaseObject
{
	WE::TriangleCollider* collider = nullptr;
	std::array<Vector3, 3> p;

public:
	void Initialize();
	void Update() override;
	void OnCollision() override { object->material.ambient = { 1,0,0 }; }
};

class Ray : public BaseObject
{
	WE::_3D::Transform transform;
	WE::RayCollider* collider = nullptr;

public:
	void Initialize();
	void Update() override;
};