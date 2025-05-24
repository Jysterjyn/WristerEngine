#pragma once
#include "BaseCamera.h"
#include <Transform.h>

namespace WristerEngine::_3D
{
	class FollowCamera : public BaseCamera
	{
	private:
		// í«ê’ëŒè€
		const Transform* target = nullptr;

	public:
		struct Prop : public BaseCameraProp
		{
			const Transform* target = nullptr;
		};

		FollowCamera(const Prop* prop = nullptr);
		void VirtualUpdate() override;
	};
}
