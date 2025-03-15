#include "RailCamera.h"
#include <ImGuiManager.h>
using namespace WristerEngine;
using namespace _3D;
using namespace _2D;

void RailCamera::Initialize(const Vector3& pos, const Vector3& rot)
{
	transform.translation = pos;
	transform.rotation = rot;
	camera.Initialize();
	camera.Update();

	modelManager->AddCamera("rail", &camera);
	modelManager->SetCameraName("rail");
}

void RailCamera::Update()
{
	transform.translation.z -= 0.1f;
	transform.Update();
	camera.matView = Matrix4::Inverse(transform.matWorld);
	camera.constMap->cameraPos = transform.GetWorldPosition();
	camera.matViewProjection = camera.matView * camera.matProjection;
	camera.constMap->viewproj = camera.matViewProjection;
	transform.isUpdated = false;

	ImGuiManager::PrintVector("CameraPos", transform.translation);
	ImGuiManager::PrintVector("CameraRot", transform.rotation);
}