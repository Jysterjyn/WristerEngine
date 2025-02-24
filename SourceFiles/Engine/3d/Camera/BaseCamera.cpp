#include "BaseCamera.h"
#include "ImGuiManager.h"
using namespace WristerEngine;
using namespace _3D;

void BaseCamera::Initialize(Transform* parent)
{
	transform.Initialize();
	transform.translation = { 0,10.0f,-10.0f };
	transform.parent = parent;
	ModelManager::SetCamera(&viewProjection);
}

void BaseCamera::Update()
{
	Vector3 eyeVec = -Vector3::MakeAxis(Axis::Z) * Matrix4::Rotate({ angle.y,angle.x }) * distance;
	transform.translation = eyeVec;
	transform.Update();
	viewProjection.target = transform.parent->GetWorldPosition();
	viewProjection.eye = transform.GetWorldPosition();
	transform.isUpdated = false;
}