#include "PlayerBullet.h"
using namespace WristerEngine::_3D;

void PlayerBullet::Initialize(const Vector3& position, const Vector3& velocity_)
{
	obj = ModelManager::GetInstance()->Create("cube");
	obj->material.ChangeTexture(0, "Models/cube/Grass.jpg");
	obj->transform.translation = position;
	velocity = velocity_;
}

void PlayerBullet::Update()
{
	obj->transform.translation += velocity;
	if (deathTimer.Update()) { obj->isDestroy = true; }
}
