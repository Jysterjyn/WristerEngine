#pragma once
#include <Vector.h>
#include <ModelManager.h>
#include <Timer.h>

class PlayerBullet
{
	static const int32_t LIFE_TIME = 60 * 5;

	WristerEngine::_3D::Object3d* obj = nullptr;
	Vector3 velocity;
	WristerEngine::FrameTimer deathTimer = LIFE_TIME;

public:
	void Initialize(const Vector3& position, const Vector3& velocity);
	void Update();
	bool IsDead() const { return obj->isDestroy; }
};

