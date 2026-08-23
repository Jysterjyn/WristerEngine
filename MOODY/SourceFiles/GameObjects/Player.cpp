#include "Player.h"
#include <CollisionInfo.h>
#include <imgui.h>

void Player::Initialize()
{
	sprite = spMan->Create({ "Nanika/Nanika.png" }, Half(WE::WIN_SIZE));
	sprite->SetCenterAnchor();

	WE::ColliderInfo info(WE::ChangeVal(CollisionAttribute::Player), WE::ChangeVal(CollisionMask::Player));
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
	ImGui::Text("Hit");
}

void Player::OnCollisionEnter()
{
	ImGui::Text("Hit Enter");
}

void Player::OnCollisionExit()
{
	ImGui::Text("Hit Exit");
}
