#include "Shake.h"

using namespace WristerEngine::_3D;

std::unique_ptr<Shake> Shake::Create(const Prop& shakeProp)
{
	// �C���X�^���X����
	std::unique_ptr<Shake> instance = std::make_unique<Shake>();
	instance->SetShakeRange(shakeProp.range);
	instance->easing.Initialize(shakeProp.time, shakeProp.easingType);
	return instance;
}

void Shake::SetShakeRange(const Vector3& shakeRange_)
{
	// �����̐���
	rand[Axis::X] = Random_Float(-shakeRange_.x, shakeRange_.x);
	rand[Axis::Y] = Random_Float(-shakeRange_.y, shakeRange_.y);
	rand[Axis::Z] = Random_Float(-shakeRange_.z, shakeRange_.z);
}

void Shake::Start()
{
	isWhile = true;
	easing.Restart();
}

Vector3 Shake::Update()
{
	// �C�[�W���O���ĂȂ��Ȃ�l��0
	if (!isWhile) { return {}; }

	Vector3 shake = { rand[Axis::X](),rand[Axis::Y](),rand[Axis::Z]() }; // �V�F�C�N�ʂ�ݒ�
	shake *= Easing::MAX - easing.Update(); // �V�F�C�N���C�[�W���O�Ō���������

	// �V�F�C�N�I��
	if (easing.IsFinish()) { isWhile = false; }

	return shake;
}
