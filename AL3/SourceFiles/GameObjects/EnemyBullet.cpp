#include "EnemyBullet.h"
#include <Player.h>
#include <Quaternion.h>
using namespace WristerEngine::_3D;

void EnemyBullet::Initialize(const Vector3& position, const Vector3& velocity_)
{
	obj = ModelManager::GetInstance()->Create("cube");
	obj->material.ChangeTexture(0, "Models/cube/Dirt.jpg");
	obj->transform.translation = position;
	obj->transform.scale = { 0.5f,0.5f,3.0f };
	velocity = velocity_;

	obj->transform.rotation.y = std::atan2(velocity.x, velocity.z);
	float velocityXZ = Length(Vector2(velocity.x, velocity.z));
	obj->transform.rotation.x = std::atan2(-velocity.y, velocityXZ);
}

void EnemyBullet::Update()
{
	//Vector3 toPlayer = Normalize(player->GetWorldPosition() - obj->transform.GetWorldPosition());
	//velocity.Normalize();
	//Quaternion q0, q1;
	//q0 = { 10000,velocity };
	//q1 = { 10000,toPlayer };

	//velocity = Slerp(q0, q1, 0.1f).GetImaginary() * 1.0f;
	obj->transform.translation += velocity;

	if (deathTimer.Update()) { obj->isDestroy = true; }
}
