#include "Skydome.h"
#include "ModelManager.h"

void WristerEngine::Skydome::Initialize(const std::string& modelName,float scale)
{
	object = _3D::ModelManager::GetInstance()->Create(modelName, true);
	object->transform.scale *= scale;
}