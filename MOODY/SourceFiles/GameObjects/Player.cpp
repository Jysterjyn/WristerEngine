#include "Player.h"
#include <CollisionInfo.h>

void Player::Initialize()
{
	sprite = spMan->Create({ "Nanika/Nanika.png" }, Half(WE::WIN_SIZE));
	sprite->SetCenterAnchor();

	transform = sprite;
	shapeType = WE::_2D::CollisionShapeType::Sphere;
	colliderName = "Player";
	attribute = ChangeVal(CollisionAttribute::Player);
	mask = ChangeVal(CollisionMask::Player);
	SetRadius(Half(sprite->size.x));
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

void Player::OnCollision(TestCircleCollider* other)
{
}