#include "Skydome.h"
#include "ModelManager.h"

void WristerEngine::Skydome::Initialize(float scale)
{
	object = _3D::ModelManager::Create("skydome", true);
	object->worldTransform->scale *= scale;
}