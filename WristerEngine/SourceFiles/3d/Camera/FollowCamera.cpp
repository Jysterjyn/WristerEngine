#include "FollowCamera.h"
using namespace WE;
using namespace _3D;

Vector3 FollowCamera::CalculateOffset() const
{
	// 追従対象からのオフセット(無回転時)
	Vector3 offset = posOffset;
	// 回転行列の合成
	Matrix4 rotMat = Matrix4::Rotate(transform.rotation);
	// オフセットをカメラの回転に合わせて回転させる
	offset *= rotMat;

	return offset;
}

FollowCamera::FollowCamera(const Prop* prop)
{
	calMode = CalMode::Transform;
	if (prop)
	{
		SetTarget(prop->target);
		rotSpeed = prop->rotSpeed;
		posOffset = prop->posOffset;
		lerpPosRate = prop->lerpPosRate;
		lerpAngleRate = prop->lerpAngleRate;
	}
	SetTransform(&transform);
}

void FollowCamera::VirtualUpdate()
{
	if (lockOn && lockOn->GetTarget())
	{
		Vector3 lockOnPos = lockOn->GetTarget()->GetCenterPos();
		Vector3 sub = lockOnPos - transform.translation;
		transform.rotation.y = std::atan2(sub.x, sub.z);
	}
	else
	{
		// 旋回操作
		if (input->IsConnectGamePad())
		{
			destinationAngleY += (float)input->ConRStick(0, rotSpeed).x;
			// 右スティック押し込みでリセット
			if (input->IsTrigger(0, JoyPad::Rstick))
			{
				destinationAngleY = targetObject->rotation.y;
			}
		}
	}

	// 最短角度補間
	transform.rotation.y = LerpShortAngle(transform.rotation.y, destinationAngleY, lerpAngleRate);

	// 追従対象がいれば
	if (targetObject)
	{
		// 追従座標の補間
		interTarget = Lerp(interTarget, targetObject->translation, lerpPosRate);

		// 追従対象からのオフセット
		Vector3 offset = CalculateOffset();

		// カメラ座標
		transform.translation = interTarget + offset;
	}

	transform.Update();
}

void FollowCamera::Reset()
{
	// 追従対象がいれば
	if (targetObject)
	{
		// 追従座標・角度の初期化
		interTarget = targetObject->translation;
		transform.rotation.y = targetObject->rotation.y;
	}
	destinationAngleY = transform.rotation.y;

	// 追従対象からのオフセット
	Vector3 offset = CalculateOffset();
	transform.translation = interTarget + offset;
}

void WristerEngine::_3D::FollowCamera::SetTarget(const Transform* target_)
{
	targetObject = target_;
	Reset();
}
