#pragma once
#include <ModelManager.h>
#include <Input.h>
#include <PlayerBullet.h>
#include <list>

class Player : WristerEngine::SphereCollider
{
	WristerEngine::_3D::Object3d* obj = nullptr;
	WristerEngine::Input* input = WristerEngine::Input::GetInstance();
	std::list<std::unique_ptr<PlayerBullet>> bullets;

	void Move();
	void Rotate();
	void Attack();

public:
	void Initialize();
	void Update();
	void SetParent(WristerEngine::_3D::Transform* parent) { transform->parent = parent; }
	Vector3 GetWorldPosition() const { return obj->transform.GetWorldPosition(); }
};
