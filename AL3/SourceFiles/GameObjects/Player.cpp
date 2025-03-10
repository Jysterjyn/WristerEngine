#include "Player.h"
#include <ImGuiManager.h>
#include <algorithm>
using namespace WristerEngine;

void Player::Initialize()
{
	obj = _3D::ModelManager::Create("cube");
}

void Player::Update()
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

void Player::Draw()
{
}
