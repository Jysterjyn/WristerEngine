#include "Player.h"
#include <ImGuiManager.h>
#include <ImGui.h>
#include <algorithm>
using namespace WristerEngine;

void Player::Move()
{
	Vector3 move;
	const float CHARACTER_SPEED = 0.2f;

	move.x = input->Move(Key::Right, Key::Left, CHARACTER_SPEED);
	move.y = input->Move(Key::Up, Key::Down, CHARACTER_SPEED);

	Vector3& pos = obj->transform.translation;
	pos += move;

	const Vector2 MOVE_LIMIT = { 12.5f,6.5f };

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
	Vector3 velocity(0, 0, BULLET_SPEED);
	velocity *= Matrix4::Rotate(obj->transform.rotation);

	std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
	newBullet->Initialize(obj->transform.translation, velocity);
	bullets.push_back(std::move(newBullet));
}

void Player::Initialize()
{
	obj = _3D::ModelManager::GetInstance()->Create("cube");
}

void Player::Update()
{
	bullets.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		if (bullet->IsDead()) { return true; }
		return false;
		});

	Rotate();
	Move();
	Attack();
	for (auto& bullet : bullets) { bullet->Update(); }
}