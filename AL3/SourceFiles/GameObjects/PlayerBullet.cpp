#include "PlayerBullet.h"
using namespace WristerEngine::_3D;
using namespace WristerEngine::_2D;

void PlayerBullet::Initialize(const Vector3& position)
{
	obj = ModelManager::GetInstance()->Create("cube");
	obj->material.ChangeTexture(0, "Models/cube/Grass.jpg");
	obj->transform.translation = position;
}

void PlayerBullet::Update()
{
}
