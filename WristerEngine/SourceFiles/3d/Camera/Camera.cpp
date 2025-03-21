#include "Camera.h"
using namespace WristerEngine::_3D;

void Camera::VirtualUpdate()
{
	// ƒrƒ…[s—ñ‚ğ‹‚ß‚é
	std::array<Vector3, 3> axis = CalculateAxis(sTarget - sEye, &up);
	Vector3 cameraMove;
	for (size_t i = 0; i < axis.size(); i++) { cameraMove[i] = Dot(sEye, axis[i]); }
	matView = Matrix4::CreateFromVector(axis[(int)Axis::X], axis[(int)Axis::Y], axis[(int)Axis::Z]);
	matView = Matrix4::Inverse(matView);
	for (size_t i = 0; i < axis.size(); i++) { matView.m[3][i] = -cameraMove[i]; }
}
