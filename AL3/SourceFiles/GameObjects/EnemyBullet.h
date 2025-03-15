#pragma once
#include <Vector.h>
#include <ModelManager.h>
#include <Timer.h>
#include <Collider.h>

class Player;

class EnemyBullet : WristerEngine::SphereCollider
{
	static const int32_t LIFE_TIME = 60 * 5;

	WristerEngine::_3D::Object3d* obj = nullptr;
	Vector3 velocity;
	WristerEngine::FrameTimer deathTimer = LIFE_TIME;
	const Player* player = nullptr;

public:
	void Initialize(const Vector3& position, const Vector3& velocity);
	void Update();
	void SetPlayer(const Player* player_) { player = player_; }
	bool IsDead() const { return obj->isDestroy; }
	void OnCollision([[maybe_unused]] WristerEngine::SphereCollider* collider) override { obj->isDestroy = true; }
};

