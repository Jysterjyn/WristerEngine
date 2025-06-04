#include "LockOn.h"
#include <Input.h>
#include <FollowCamera.h>

void LockOn::Initialize(std::initializer_list<const std::string> textureNames)
{
	lockOnMark = WE::_2D::Sprite::Create(textureNames);
	camera = WE::_3D::CameraManager::GetInstance()->Get();
}

void LockOn::Update()
{
	WE::Input* input = WE::Input::GetInstance();

	if (target)
	{
		if (input->IsTrigger(0, WE::JoyPad::X)) { target = nullptr; }
		else if (IsOutRange(target)) { target = nullptr; }
	}
	else
	{
		if (input->IsTrigger(0, WE::JoyPad::X)) { Search(); }
	}

	if (target) { lockOnMark->position = To2DVector(target->GetCenterPos()); }
}

void LockOn::Search()
{
	std::list<std::pair<float, const LockOnObject*>> targets;

	for (auto& object : *objects)
	{
		float positionViewZ = 0;
		if (!IsOutRange(object.get(), &positionViewZ))
		{
			targets.emplace_back(std::make_pair(positionViewZ, object.get()));
		}
	}

	target = nullptr;
	if (!targets.empty())
	{
		targets.sort([](auto& pair1, auto& pair2) { return pair1.first < pair2.first; });
		target = targets.front().second;
	}
}

bool LockOn::IsOutRange(const LockOnObject* object, float* positionViewZ)
{
	Vector3 positionWorld = object->GetCenterPos();
	Vector3 positionView = positionWorld * camera->GetViewMatrix();
	if (positionViewZ) { *positionViewZ = positionView.z; }

	if (minDistance <= positionView.z && positionView.z <= maxDistance)
	{
		float arcTangent = std::atan2(std::sqrt(
			positionView.x * positionView.x + positionView.y * positionView.y),
			positionView.z);

		if (std::abs(arcTangent) <= angleRange) { return false; }
	}
	return true;
}

void LockOn::Draw()
{
	if (target) { lockOnMark->Draw(); }
}

Vector3 LockOn::GetTargetPosition() const
{
	if (target) { return target->GetCenterPos(); }
	return Vector3();
}
