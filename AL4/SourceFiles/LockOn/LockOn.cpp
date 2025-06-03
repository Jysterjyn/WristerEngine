#include "LockOn.h"
#include <Input.h>

void LockOn::Initialize()
{
	lockOnMark = WE::_2D::Sprite::Create({ "Reticle.png" });
	camera = WE::_3D::CameraManager::GetInstance()->Get();
}

void LockOn::Update(const std::list<std::unique_ptr<Enemy>>& enemies)
{
	WE::Input* input = WE::Input::GetInstance();

	if (target)
	{
		if (input->IsTrigger(0, WE::JoyPad::X)) { target = nullptr; }
		else if (IsOutRange()) { target = nullptr; }
	}
	else
	{
		if (input->IsTrigger(0, WE::JoyPad::X)) { Search(enemies); }
	}

	if (target) { lockOnMark->position = To2DVector(target->GetCenterPos()); }
}

void LockOn::Search(const std::list<std::unique_ptr<Enemy>>& enemies)
{
	std::list<std::pair<float, const Enemy*>> targets;

	for (auto& enemy : enemies)
	{
		Vector3 positionWorld = enemy->GetCenterPos();
		Vector3 positionView = positionWorld * camera->GetViewMatrix();

		if (minDistance <= positionView.z && positionView.z <= maxDistance)
		{
			float arcTangent = std::atan2(std::sqrt(
				positionView.x * positionView.x + positionView.y * positionView.y),
				positionView.z);

			if (std::abs(arcTangent) <= angleRange)
			{
				targets.emplace_back(std::make_pair(positionView.z, enemy.get()));
			}
		}
	}

	target = nullptr;
	if (!targets.empty())
	{
		targets.sort([](auto& pair1, auto& pair2) { return pair1.first < pair2.first; });
		target = targets.front().second;
	}
}

bool LockOn::IsOutRange()
{
	Vector3 positionWorld = target->GetCenterPos();
	Vector3 positionView = positionWorld * camera->GetViewMatrix();

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
	if (target)
	{
		lockOnMark->Draw();
	}
}
