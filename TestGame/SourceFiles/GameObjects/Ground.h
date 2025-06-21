#pragma once
#include "Object3d.h"
#include <Collider.h>
#include <ModelManager.h>

class Ground : public WE::Collider
{
	WE::_3D::ModelManager* mm = WE::_3D::ModelManager::GetInstance();
	WE::_3D::Object3d* object = nullptr;
	WE::MeshCollider* collider = nullptr;

public:
	void Initialize(const Vector3& pos, float scale);
	WE::_3D::Transform& GetTransform() { return object->transform; }
};
