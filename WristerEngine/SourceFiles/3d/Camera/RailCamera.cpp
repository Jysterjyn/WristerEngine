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
	std::vector<Vector3> controlPoints =
	{
		{0,  0,  0},
		{10, 10, 0},
		{10, 15, 0},
		{20, 15, 0},
		{20, 0,  0},
		{30, 0,  0},
	};

	const float dt = 1.0f / 600.0f;
	eye = SplineCurve(controlPoints, t);
	target = SplineCurve(controlPoints, std::clamp(t + dt, 0.0f, 1.0f));
	t += dt;

	transform.translation = eye;

	Vector3 forward = Normalize(target - eye);
	transform.rotation.y = std::atan2(forward.x, forward.z);
	float velocityXZ = Length(Vector2(forward.x, forward.z));
	transform.rotation.x = std::atan2(-forward.y, velocityXZ);
}
