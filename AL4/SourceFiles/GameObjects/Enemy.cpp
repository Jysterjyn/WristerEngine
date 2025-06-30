#include "Enemy.h"
#include <CollisionConfig.h>
#include <imgui.h>

uint32_t Enemy::nextSerialNumber = 0;

Enemy::Enemy()
{
	serialNumber = nextSerialNumber;
	++nextSerialNumber;
}

void Enemy::Initialize(const std::string& modelGroupName)
{
	WE::Collider::Initialize("Enemy");
	GetGroup()->SetAttribute(ChangeVal(CollisionAttribute::Enemy));

	BaseCharacter::Initialize(modelGroupName);
	collider->SetAttribute(ChangeVal(CollisionAttribute::Enemy));

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
	// 速さ
	const float ROT_SPEED = Angle(1);
	const float SPEED = 0.1f;

	// 移動量
	rootPos.rotation.y += ROT_SPEED;
	Vector3 move = { 0.0f,0.0f,1.0f };

	// 移動ベクトルをY軸周りの角度だけ回転する
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

Vector3 Enemy::GetCenterPos() const
{
	const Vector3 offset = { 0,0.5f,0.0f };
	return offset + rootPos.GetWorldPosition();
}

void Enemy::OnCollision([[maybe_unused]] WE::ColliderGroup* collisionGroup)
{
}
