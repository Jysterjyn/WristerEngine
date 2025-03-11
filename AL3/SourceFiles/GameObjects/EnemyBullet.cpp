#include "EnemyBullet.h"
using namespace WristerEngine::_3D;

void EnemyBullet::Initialize(const Vector3& position, const Vector3& velocity_)
{
	obj = ModelManager::GetInstance()->Create("cube");
	obj->material.ChangeTexture(0, "Models/cube/Dirt.jpg");
	obj->transform.translation = position;
	velocity = velocity_;
}

void EnemyBullet::Update()
{
	obj->transform.translation += velocity;
	if (deathTimer.Update()) { obj->isDestroy = true; }
}
