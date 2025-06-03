#include "Enemy.h"

void Enemy::Initialize(const std::string& modelGroupName)
{
	BaseCharacter::Initialize(modelGroupName);

	rootPos.scale *= 0.4f;

	for (auto& o : objects)
	{
		o.second->material.ambient = { 0,0,0 };
		o.second->transform.parent = &rootPos;
	}

	InitializeWalkingGimmick();
}

void Enemy::Move()
{
	// ‘¬‚³
	const float ROT_SPEED = Angle(1);
	const float SPEED = 0.1f;

	// ˆÚ“®—Ê
	rootPos.rotation.y += ROT_SPEED;
	Vector3 move = { 0.0f,0.0f,1.0f };

	// ˆÚ“®ƒxƒNƒgƒ‹‚ðYŽ²Žü‚è‚ÌŠp“x‚¾‚¯‰ñ“]‚·‚é
	Matrix4 rotMat = Matrix4::RotateY(rootPos.rotation.y);
	move *= rotMat;

	rootPos.translation += move * SPEED;
}

void Enemy::InitializeWalkingGimmick()
{
	walkingParameter = 0.0f;
}

void Enemy::UpdateWalkingGimmick()
{
	const float STEP = 2.0f * PI / cycle;

	walkingParameter += STEP;
	walkingParameter = std::fmod(walkingParameter, 2.0f * PI);
	float rotateSpd = std::asin(std::sin(walkingParameter)) * amplitude;
	objects["shoesL"]->transform.rotation.x = rotateSpd;
	objects["shoesR"]->transform.rotation.x = -rotateSpd;
}

void Enemy::Update()
{
	Move();
	UpdateWalkingGimmick();
	BaseCharacter::Update();
}