#pragma once
#include <Vector.h>
#include <ModelManager.h>

class PlayerBullet
{
	WristerEngine::_3D::Object3d* obj = nullptr;

public:
	void Initialize(const Vector3& position);
	void Update();
	void Destroy() { obj->isDestroy = true; }
};

