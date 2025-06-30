#pragma once
#include <Sprite.h>
#include <CameraManager.h>

namespace WristerEngine::_3D
{
	// ロックオン対象の基底クラス
	class LockOnObject
	{
	public:
		virtual ~LockOnObject() = default;
		virtual void Update() = 0;
		virtual Vector3 GetCenterPos() const = 0;
	};

	// ロックオン
	class LockOn
	{
		// ロックオンマーク用スプライト
		WE::_2D::Sprite* lockOnMark = nullptr;
		// ロックオン対象
		const LockOnObject* target = nullptr;
		const WE::_3D::BaseCamera* camera = nullptr;
		const std::list<std::unique_ptr<LockOnObject>>* objects = nullptr;

		// 最小距離
		float minDistance = 10.0f;
		// 最大距離
		float maxDistance = 30.0f;
		// 角度範囲
		float angleRange = Angle(20);

		void Search();
		bool IsOutRange(const LockOnObject* object, float* positionViewZ = nullptr);

	public:
		void Initialize(std::initializer_list<const std::string> textureNames);
		void Update();
		void Draw();
		void SetLockOnObjects(const std::list<std::unique_ptr<LockOnObject>>* objects_) { objects = objects_; }
		Vector3 GetTargetPosition() const;
		bool ExistTarget() const { return target ? true : false; }
	};
}
