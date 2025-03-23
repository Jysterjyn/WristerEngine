#include "RailCamera.h"
#include <ImGuiManager.h>
#include <ImGui.h>
using namespace WristerEngine;
using namespace _3D;
using namespace _2D;

RailCamera::RailCamera(const Vector3& pos, const Vector3& rot)
{
	transform.translation = pos;
	transform.rotation = rot;
	SetTransform(&transform);
}

void RailCamera::VirtualUpdate()
{
	transform.translation.z -= 0.1f;
	transform.Update();
	transform.isUpdated = false;

	//ImGuiManager::PrintVector("CameraPos", transform.translation);
	//ImGuiManager::PrintVector("CameraRot", transform.rotation);
}
