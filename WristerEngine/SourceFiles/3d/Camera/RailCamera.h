#pragma once
#include <ModelManager.h>

namespace WristerEngine::_3D
{
	class RailCamera : public BaseCamera
	{
		ModelManager* modelManager = ModelManager::GetInstance();
		Transform transform;
		std::vector<Vector3> controlPoints;
		float t = 0;

	public:
		struct Prop
		{
			Vector3 pos, rot;
			std::vector<Vector3> controlPoints;
		};

		RailCamera(const Prop* prop = nullptr);
		Transform* GetTransform() { return &transform; }

		// BaseCamera ����Čp������܂���
		void VirtualUpdate() override;
	};
}