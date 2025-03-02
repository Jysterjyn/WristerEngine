#include "CameraShake.h"

using namespace WristerEngine::_3D;

std::unique_ptr<CameraShake> CameraShake::Create(const Prop& shakeProp)
{
	// インスタンス生成
	std::unique_ptr<CameraShake> instance = std::make_unique<CameraShake>();
	instance->SetShakeRange(shakeProp.range);
	instance->easing.Initialize(shakeProp.time, shakeProp.easingType);
	return instance;
}

void WristerEngine::_3D::CameraShake::SetShakeRange(const Vector3& shakeRange_)
{
	// 乱数の生成
	rand[Axis::X] = Random_Float(-shakeRange_.x, shakeRange_.x);
	rand[Axis::Y] = Random_Float(-shakeRange_.y, shakeRange_.y);
	rand[Axis::Z] = Random_Float(-shakeRange_.z, shakeRange_.z);
}

void WristerEngine::_3D::CameraShake::Start()
{
	isWhile = true;
	easing.Restart();
}

Vector3 WristerEngine::_3D::CameraShake::Update()
{
	// イージングしてないなら値は0
	if (!isWhile) { return {}; }

	Vector3 shake = { rand[Axis::X](),rand[Axis::Y](),rand[Axis::Z]() }; // シェイク量を設定
	shake *= Easing::MAX - easing.Update(); // シェイクをイージングで減衰させる

	// シェイク終了
	if (easing.IsFinish()) { isWhile = false; }

	return shake;
}
