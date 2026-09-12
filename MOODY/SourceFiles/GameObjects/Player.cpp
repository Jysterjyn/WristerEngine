#include "Player.h"
#include <CollisionInfo.h>
#include <imgui.h>
#include <algorithm>

void Player::Initialize()
{
	WE::_2D::SpriteProp prop;
	prop.fileNames.push_back("Nanika/Nanika.png");
	prop.pos = Half(WE::WIN_SIZE);
	sprite = spMan->Create(prop);
	sprite->SetCenterAnchor();

	WE::ColliderInfo info(WE::ChangeVal(CollisionAttribute::Player), WE::ChangeVal(CollisionMask::Player));
	Collider::Initialize("Player", info);
	WE::_2D::CircleCollider* collider = AddCollider<WE::_2D::CircleCollider>();
	collider->SetTransform(sprite);
	collider->SetRadius(Half(sprite->size.x));
}

void Player::Update()
{
	const ScreenSpeed MOVE_SPD(10.0f);
	sprite->position.y += input->Move(WE::Key::Down, WE::Key::Up, MOVE_SPD);
	sprite->position.y = std::clamp(sprite->position.y, Half(sprite->size.y), WE::WIN_SIZE.y - Half(sprite->size.y));
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
