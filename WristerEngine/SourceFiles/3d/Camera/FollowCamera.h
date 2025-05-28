#pragma once
#include "BaseCamera.h"
#include <Transform.h>
#include <Input.h>

namespace WristerEngine::_3D
{
	class FollowCamera : public BaseCamera
	{
	private:
		// 追従対象
		const Transform* targetObject = nullptr;
		Transform transform;
		float rotSpeed = 0;
		// 追従対象からカメラまでのオフセット(0°の時の値)
		Vector3 posOffset;
		Input* input = Input::GetInstance();

	public:
		struct Prop : public BaseCameraProp
		{
			const Transform* target = nullptr;
			float rotSpeed = Angle(2);
			Vector3 posOffset;
		};

		FollowCamera(const Prop* prop = nullptr);
		void VirtualUpdate() override;
	};
}
