#pragma once
#include <ModelManager.h>
#include <Input.h>
#include <PlayerBullet.h>
#include <list>
#include <Sprite.h>

class Player : WristerEngine::SphereCollider
{
	WristerEngine::_3D::Object3d* obj = nullptr;
	WristerEngine::_3D::Object3d* obj3DReticle;
	WristerEngine::Input* input = WristerEngine::Input::GetInstance();
	std::list<std::unique_ptr<PlayerBullet>> bullets;
	WristerEngine::_2D::Sprite* sprite2DReticle = nullptr;

	void Move();
	void Rotate();
	void Attack();

public:
	void Initialize();
	void Update();
	void Draw() { sprite2DReticle->Draw(); }
	void SetParent(WristerEngine::_3D::Transform* parent) { transform->parent = parent; }
	Vector3 GetWorldPosition() const { return obj->transform.GetWorldPosition(); }
};
