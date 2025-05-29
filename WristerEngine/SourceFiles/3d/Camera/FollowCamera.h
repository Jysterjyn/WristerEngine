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
		Vector3 interTarget;
		float destinationAngleY = 0; // 目標角度
		float lerpPosRate = 0;
		float lerpAngleRate = 0;

		Vector3 CalculateOffset() const;

	public:
		struct Prop : public BaseCameraProp
		{
			const Transform* target = nullptr;
			float rotSpeed = Angle(2);
			Vector3 posOffset;
			float lerpPosRate = 0.3f;
			float lerpAngleRate = 0.3f;
		};

		FollowCamera(const Prop* prop = nullptr);
		void VirtualUpdate() override;
		void Reset();
		// setter
		void SetTarget(const Transform* target_);
		void SetLerpPosRate(float lerpPosRate_) { lerpPosRate = lerpPosRate_; }
		void SetLerpAngleRate(float lerpAngleRate_) { lerpAngleRate = lerpAngleRate_; }
		void SetRotSpeed(float rotSpeed_) { rotSpeed = rotSpeed_; }
	};
}
