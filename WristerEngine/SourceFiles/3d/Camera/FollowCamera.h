#pragma once
#include "BaseCamera.h"
#include <Transform.h>
#include <Input.h>

namespace WristerEngine::_3D
{
	class FollowCamera : public BaseCamera
	{
	private:
		// í«è]ëŒè€
		const Transform* targetObject = nullptr;
		Transform transform;
		Input* input = Input::GetInstance();

	public:
		struct Prop : public BaseCameraProp
		{
			const Transform* target = nullptr;
		};

		FollowCamera(const Prop* prop = nullptr);
		void VirtualUpdate() override;
	};
}
