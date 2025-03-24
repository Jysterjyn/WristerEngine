#include "Player.h"
#include <ImGuiManager.h>
#include <ImGui.h>
#include <algorithm>
#include <DirectXCommon.h>
using namespace WristerEngine;

void Player::Move()
{
	Vector3 move;
	const float CHARACTER_SPEED = 0.2f;

	move.x = input->Move(Key::Right, Key::Left, CHARACTER_SPEED);
	move.y = input->Move(Key::Up, Key::Down, CHARACTER_SPEED);

	Vector3& pos = obj->transform.translation;
	pos += move;

	const Vector2 MOVE_LIMIT = { 34.5f,18.5f };

	pos.x = std::clamp(pos.x, -MOVE_LIMIT.x, MOVE_LIMIT.x);
	pos.y = std::clamp(pos.y, -MOVE_LIMIT.y, MOVE_LIMIT.y);

	_2D::ImGuiManager::SliderVector("Player", pos, -MOVE_LIMIT.x, MOVE_LIMIT.x);
}

void Player::Rotate()
{
	const float ROT_SPEED = 0.02f;
	obj->transform.rotation.y += input->Move(Key::D, Key::A, ROT_SPEED);
}

void Player::Attack()
{
	if (!input->IsTrigger(Key::Space)) { return; }

	const float BULLET_SPEED = 1.0f;
	Vector3 velocity = transform3DReticle.GetWorldPosition() - obj->transform.GetWorldPosition();
	velocity = Normalize(velocity) * BULLET_SPEED;
	std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
	newBullet->Initialize(obj->transform.GetWorldPosition(), velocity);
	bullets.push_back(std::move(newBullet));
}

void Player::Initialize()
{
	obj = _3D::ModelManager::GetInstance()->Create("cube");
	transform = &obj->transform;
	transform->translation.z = 50.0f;
	collisionAttribute = CollisionAttribute::Player;
	collisionMask = CollisionMask::Player;
	sprite2DReticle = _2D::Sprite::Create({ "Reticle.png" });
	sprite2DReticle->SetCenterAnchor();
	sprite2DReticle->SetCenterPos();
}

void Player::Update()
{
	bullets.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		if (bullet->IsDead()) { return true; }
		return false;
		});

	obj->transform.Update();
	Vector3 offset = { 0,0,1 };
	offset *= Matrix4::Rotate(obj->transform.rotation);
	offset = Normalize(offset) * 50.0f;
	transform3DReticle.translation = obj->transform.GetWorldPosition() + offset;
	transform3DReticle.Update();
	transform3DReticle.isUpdated = false;

	Vector3 positionReticle = transform3DReticle.GetWorldPosition();
	sprite2DReticle->position = To2DVector(positionReticle);

	Rotate();
	Move();
	Attack();
	for (auto& bullet : bullets) { bullet->Update(); }
}