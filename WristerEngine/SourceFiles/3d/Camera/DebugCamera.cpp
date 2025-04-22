#include "DebugCamera.h"
#include "Input.h"
#include "WindowsAPI.h"
using namespace WristerEngine;
using namespace _3D;

DebugCamera::DebugCamera(const Prop* prop)
{
	if(prop)
	{
		distance = prop->distance;
		target = eye = prop->targetPos;
		mouseMoveDec = prop->mouseMoveDec;
		wheelSpdDec = prop->wheelSpdDec;
	}
	eye.z -= distance;
}

void DebugCamera::VirtualUpdate()
{
	bool dirty = false;
	Vector2 angle;
	Vector2 scale = { 1.0f / WIN_SIZE.x, 1.0f / WIN_SIZE.y };
	Input* input = Input::GetInstance();

	// �}�E�X�̓��͂��擾
	Input::MouseMove mouseMove = input->GetMouseMove();

	// �}�E�X�̍��{�^����������Ă�����J��������]������
	if (input->IsInput(Mouse::Right))
	{
		angle = -Vector2((float)mouseMove.lY * scale.x, (float)mouseMove.lX * scale.y) * PI;
		dirty = true;
	}

	// �}�E�X�̒��{�^����������Ă�����J�����𕽍s�ړ�������
	if (input->IsInput(Mouse::Middle))
	{
		Vector3 move = Vector3(-(float)mouseMove.lX, (float)mouseMove.lY) / mouseMoveDec;
		move = RotateVector(move, rotQ);

		CameraMove(move);
		dirty = true;
	}

	// �z�C�[�����͂ŋ�����ύX
	if (mouseMove.lZ != 0)
	{
		distance -= mouseMove.lZ / wheelSpdDec;
		distance = max(distance, 1.0f);
		dirty = true;
	}

	if (!dirty) { return; }
	// �ǉ���]����Quaternion�𐶐�
	Quaternion rotQNew;
	rotQNew = MakeAxisAngle(Vector3::MakeAxis(Axis::Y), -angle.y);
	rotQNew *= MakeAxisAngle(Vector3::MakeAxis(Axis::X), -angle.x);

	// �ݐς�Quaternion������
	rotQ *= rotQNew;

	// �����_���王�_�ւ̃x�N�g���ƁA������x�N�g��
	Vector3 vTargetEye = { 0.0f, 0.0f, -distance };
	Vector3 vUp = Vector3::MakeAxis(Axis::Y);

	// �x�N�g������]
	vTargetEye = RotateVector(vTargetEye, rotQ);
	vUp = RotateVector(vUp, rotQ);

	// �����_���炸�炵���ʒu�Ɏ��_���W������
	eye = target + vTargetEye;
	up = vUp;
}