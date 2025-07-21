#include "LockOn.h"
#include <Input.h>
#include <FollowCamera.h>

using namespace WE;
using namespace _3D;

void LockOn::Initialize(std::initializer_list<const std::string> textureNames)
{
	lockOnMark = _2D::Sprite::Create(textureNames);
	camera = _3D::CameraManager::GetInstance()->Get();
}

void LockOn::Update()
{
	Input* input = Input::GetInstance();

	// ロックオン状態なら
	if (target)
	{
		// ロックオンボタンをトリガーしたら
		if (input->IsTrigger(0, JoyPad::X))
		{
			// ロックオンを外す
			target = nullptr; 
		}
		// 範囲外判定
		else if (IsOutRange(target)) 
		{ 
			// ロックオンを外す
			target = nullptr;
		}
	}
	else
	{
		// ロックオンボタンをトリガーしたらロックオン対象の検索
		if (input->IsTrigger(0, JoyPad::X)) { Search(); }
	}

	// ロックオン状態なら
	if (target)
	{
		// スプライトの座標を設定
		lockOnMark->position = To2DVector(target->GetCenterPos());
	}
}

void LockOn::Search()
{
	// 目標
	std::list<std::pair<float, const LockOnObject*>> targets;
	// 全ての対象に対して順にロックオン判定
	for (auto& object : *objects)
	{
		float positionViewZ = 0;
		if (!IsOutRange(object.get(), &positionViewZ))
		{
			targets.emplace_back(std::make_pair(positionViewZ, object.get()));
		}
	}

	// ロックオン対象をリセット
	target = nullptr;
	if (!targets.empty())
	{
		// 距離で昇順にソート
		targets.sort([](auto& pair1, auto& pair2) { return pair1.first < pair2.first; });
		// ソートの結果一番近い対象をロックオン対象とする
		target = targets.front().second;
	}
}

bool LockOn::IsOutRange(const LockOnObject* object, float* positionViewZ)
{
	// 対象のロックオン座標取得
	Vector3 positionWorld = object->GetCenterPos();
	// ワールド→ビュー座標変換
	Vector3 positionView = positionWorld * camera->GetViewMatrix();
	if (positionViewZ) { *positionViewZ = positionView.z; }

	// 距離条件チェック
	if (minDistance <= positionView.z && positionView.z <= maxDistance)
	{
		// カメラ前方との角度を計算
		float arcTangent = std::atan2(std::sqrt(
			positionView.x * positionView.x + positionView.y * positionView.y),
			positionView.z);

		// 角度条件チェック(コーンに収まっているか)
		if (std::abs(arcTangent) <= angleRange)
		{
			// 範囲外ではない
			return false; 
		}
	}
	// 範囲外である
	return true;
}

void LockOn::Draw()
{
	if (target) { lockOnMark->Draw(); }
}

Vector3 LockOn::GetTargetPosition() const
{
	if (target) { return target->GetCenterPos(); }
	return Vector3();
}
