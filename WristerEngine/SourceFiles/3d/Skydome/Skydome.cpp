#include "Skydome.h"
#include "ModelManager.h"

void WristerEngine::Skydome::Initialize(float scale)
{
	object = _3D::ModelManager::GetInstance()->Create("skydome", true);
	object->transform.scale *= scale;
}