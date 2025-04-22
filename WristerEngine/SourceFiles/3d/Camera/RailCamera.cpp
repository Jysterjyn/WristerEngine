#include "RailCamera.h"
#include <ImGuiManager.h>
#include <ImGui.h>
using namespace WristerEngine;
using namespace _3D;
using namespace _2D;

RailCamera::RailCamera(const Prop* prop)
{
	if(prop)
	{
		transform.translation = prop->pos;
		transform.rotation = prop->rot;
		controlPoints = prop->controlPoints;
	}
	SetTransform(&transform);
}

void RailCamera::VirtualUpdate()
{
	if (controlPoints.size() <= 1) { return; }
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
