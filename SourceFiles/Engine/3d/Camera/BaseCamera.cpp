#include "BaseCamera.h"
#include "ImGuiManager.h"
using namespace WristerEngine;
using namespace _3D;

void BaseCamera::Initialize(Transform* parent)
{
	worldTransform.Initialize();
	worldTransform.translation = { 0,10.0f,-10.0f };
	worldTransform.parent = parent;
	ModelManager::SetCamera(&viewProjection);
}

void BaseCamera::Update()
{
	Vector3 eyeVec = -Vector3::MakeAxis(Axis::Z) * Matrix4::Rotate({ angle.y,angle.x }) * distance;
	worldTransform.translation = eyeVec;
	worldTransform.Update();
	viewProjection.target = worldTransform.parent->GetWorldPosition();
	viewProjection.eye = worldTransform.GetWorldPosition();
	worldTransform.isUpdated = false;
}