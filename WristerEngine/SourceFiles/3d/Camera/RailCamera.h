#pragma once
#include <ModelManager.h>

namespace WristerEngine::_3D
{
	class RailCamera : public BaseCamera
	{
		ModelManager* modelManager = ModelManager::GetInstance();
		Transform transform;

	public:
		RailCamera(const Vector3& pos = {}, const Vector3& rot = {});
		Transform* GetTransform() { return &transform; }

		// BaseCamera ����Čp������܂���
		void VirtualUpdate() override;
	};
}