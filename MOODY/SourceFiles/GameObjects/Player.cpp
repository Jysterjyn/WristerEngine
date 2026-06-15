#include "Player.h"
#include <CollisionInfo.h>

void Player::Initialize()
{
	sprite = spMan->Create({ "Nanika/Nanika.png" }, Half(WE::WIN_SIZE));
	sprite->SetCenterAnchor();

	WE::ColliderInfo info = { ChangeVal(CollisionAttribute::Player), ChangeVal(CollisionMask::Player), "Player" };
	Collider::Initialize("Player", info);
	WE::_2D::CircleCollider* collider = AddCollider<WE::_2D::CircleCollider>();
	collider->SetTransform(sprite);
	collider->SetRadius(Half(sprite->size.x));
}

void Player::Update()
{
	const float MOVE_SPD = 5.0f;
	sprite->position.y += input->Move(WE::Key::Down, WE::Key::Up, MOVE_SPD);
}

void Player::Draw()
{
	sprite->Draw();
}

void Player::OnCollision()
{
}