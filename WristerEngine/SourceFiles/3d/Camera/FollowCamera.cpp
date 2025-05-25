#include "FollowCamera.h"
using namespace WE;
using namespace _3D;

FollowCamera::FollowCamera(const Prop* prop)
{
	if (prop) { targetObject = prop->target; }
	SetTransform(&transform);
}

void FollowCamera::VirtualUpdate()
{
	if (input->IsConnectGamePad())
	{
		const float ROT_SPEED = Angle(1);
		transform.rotation.y += (float)input->ConRStick(0, ROT_SPEED).x;
	}

	// 追従対象がいれば
	if (targetObject)
	{
		// 追従対象からカメラまでのオフセット(0°の時の値)
		Vector3 offset = { 0.0f,2.0f,-10.0f };

		// カメラの角度から回転行列を計算する
		Matrix4 rotMat = Matrix4::Rotate(transform.rotation);

		// オフセットをカメラの回転に合わせて回転させる
		offset *= rotMat;

		// 座標をコピーしてオフセット分ずらす
		transform.translation = targetObject->translation + offset;
	}

	transform.Update();
}