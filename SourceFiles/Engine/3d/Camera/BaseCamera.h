#pragma once
#include "ModelManager.h"

namespace WristerEngine::_3D
{
	// オブジェクトに追従するカメラ
	class BaseCamera
	{
	protected:
		float distance = 50.0f; // 注視点から視点までの距離
		Camera viewProjection;
		Vector2 angle = { 0,0.3f }; // カメラ回転量
		Transform worldTransform;

	public:
		// 仮想デストラクタ
		virtual ~BaseCamera() = default;
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="parent">ワールドトランスフォームの親ポインタ</param>
		virtual void Initialize(Transform* parent);
		// 更新
		virtual void Update();
		// シェイク開始
		void StartShake() { viewProjection.shake->Start(); }
		// getter
		Camera* GetViewProjection() { return &viewProjection; }
		const Vector2& GetAngle() { return angle; }
	};
}