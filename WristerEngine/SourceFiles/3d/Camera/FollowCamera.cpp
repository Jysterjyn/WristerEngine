#include "FollowCamera.h"
using namespace WE;
using namespace _3D;

FollowCamera::FollowCamera(const Prop* prop)
{
	if (prop) 
	{
		targetObject = prop->target; 
		rotSpeed = prop->rotSpeed;
		posOffset = prop->posOffset;
	}
	SetTransform(&transform);
}

void FollowCamera::VirtualUpdate()
{
	if (input->IsConnectGamePad())
	{
		transform.rotation.y += (float)input->ConRStick(0, rotSpeed).x;
	}

	// 追従対象がいれば
	if (targetObject)
	{
		// 追従対象からカメラまでのオフセット(0°の時の値)
		Vector3 offset = posOffset;

		// カメラの角度から回転行列を計算する
		Matrix4 rotMat = Matrix4::Rotate(transform.rotation);

		// オフセットをカメラの回転に合わせて回転させる
		offset *= rotMat;

		// 座標をコピーしてオフセット分ずらす
		transform.translation = targetObject->translation + offset;
	}

	transform.Update();
}