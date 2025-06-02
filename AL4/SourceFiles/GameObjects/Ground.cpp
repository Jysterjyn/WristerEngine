#include "Ground.h"
#include <ModelManager.h>

void Ground::Initialize(const std::string& modelName, const Vector3& scale)
{
	object = WE::_3D::ModelManager::GetInstance()->Create(modelName, true);
	object->transform.scale = scale;
	object->transform.translation.y = -2.0f;
	object->material.textures[(size_t)WE::_3D::TexType::Main].tiling = { scale.x,scale.z };
	object->material.ChangeTexture((size_t)WE::_3D::TexType::Main, "./dirt.jpg");
}