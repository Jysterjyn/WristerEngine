#pragma once
#include "Camera.h"
#include "Quaternion.h"

namespace WristerEngine::_3D
{
	// デバッグカメラ
	class DebugCamera : public Camera
	{
	private:
		float distance = 50;
		Quaternion rotQ = 1.0f;
		float mouseMoveDec = 75; // 平行移動の速度補正
		float wheelSpdDec = 50; // ズームの速度補正

	public:
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="targetPos">注視点座標</param>
		/// <param name="distance">注視点座標までの距離</param>
		/// <param name="mouseMoveDec">平行移動の速度補正</param>
		/// <param name="wheelSpdDec">ズームの速度補正</param>
		void Initialize(Vector3 targetPos = { 0,0,0 }, float distance = 50, float mouseMoveDec = 75, float wheelSpdDec = 50);
		// 更新
		void Update();
	};
}