#include "DebugCamera.h"
#include "Input.h"
#include "WindowsAPI.h"
using namespace WristerEngine;
using namespace _3D;

void DebugCamera::Initialize(Vector3 targetPos, float distance_, float mouseMoveDec_, float wheelSpdDec_)
{
	distance = distance_;
	target = eye = targetPos;
	mouseMoveDec = mouseMoveDec_;
	wheelSpdDec = wheelSpdDec_;
	eye.z -= distance;
	Camera::Initialize();
}

void DebugCamera::Update()
{
	bool dirty = false;
	Vector2 angle;
	Vector2 scale = { 1.0f / WIN_SIZE.x, 1.0f / WIN_SIZE.y };
	Input* input = Input::GetInstance();

	// マウスの入力を取得
	Input::MouseMove mouseMove = input->GetMouseMove();

	// マウスの左ボタンが押されていたらカメラを回転させる
	if (input->IsInput(Mouse::Left))
	{
		angle = -Vector2((float)mouseMove.lY * scale.x, (float)mouseMove.lX * scale.y) * PI;
		dirty = true;
	}

	// マウスの中ボタンが押されていたらカメラを平行移動させる
	if (input->IsInput(Mouse::Middle))
	{
		Vector3 move = Vector3(-(float)mouseMove.lX, (float)mouseMove.lY) / mouseMoveDec;
		move = Quaternion::RotateVector(move, rotQ);

		CameraMove(move);
		dirty = true;
	}

	// ホイール入力で距離を変更
	if (mouseMove.lZ != 0)
	{
		distance -= mouseMove.lZ / wheelSpdDec;
		distance = max(distance, 1.0f);
		dirty = true;
	}

	if (!dirty) { return; }
	// 追加回転分のQuaternionを生成
	Quaternion rotQNew;
	rotQNew = Quaternion::MakeAxisAngle(Vector3::MakeAxis(Axis::Y), -angle.y);
	rotQNew *= Quaternion::MakeAxisAngle(Vector3::MakeAxis(Axis::X), -angle.x);

	// 累積のQuaternionを合成
	rotQ *= rotQNew;

	// 注視点から視点へのベクトルと、上方向ベクトル
	Vector3 vTargetEye = { 0.0f, 0.0f, -distance };
	Vector3 vUp = Vector3::MakeAxis(Axis::Y);

	// ベクトルを回転
	vTargetEye = Quaternion::RotateVector(vTargetEye, rotQ);
	vUp = Quaternion::RotateVector(vUp, rotQ);

	// 注視点からずらした位置に視点座標を決定
	eye = target + vTargetEye;
	up = vUp;
}