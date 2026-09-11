#include "Player.h"
#include <CollisionInfo.h>
#include <imgui.h>
#include <algorithm>

void Player::Initialize()
{
	WE::_2D::SpriteProp prop;
	prop.fileNames.push_back("Nanika/Nanika.png");
	//prop.fileNames.push_back("drill_girl_walk.png");
	//prop.isAnimation = true;
	//prop.spriteNum = 6;
	//prop.interval = 1;
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
	const float MOVE_SPD = 5.0f;
	sprite->position.y += input->Move(WE::Key::Down, WE::Key::Up, MOVE_SPD);
	sprite->position.y = std::clamp(sprite->position.y, sprite->size.y / 2.0f, WE::WIN_SIZE.y - sprite->size.y / 2.0f);
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
