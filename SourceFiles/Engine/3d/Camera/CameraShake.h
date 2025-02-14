#pragma once
#include "Easing.h"
#include "Random.h"
#include "Vector.h"
#include <unordered_map>

namespace WristerEngine::_3D
{
	class CameraShake
	{
	private:
		Easing easing; // イージングによるシェイク範囲の減衰
		bool isWhile = false; // シェイク中か
		std::unordered_map<Axis, Random_Float> rand{}; // 乱数

	public:
		// シェイク情報
		struct Prop
		{
			Vector3 range; // シェイクの範囲
			int time; // シェイク時間
			Easing::Type easingType; // シェイク減衰イージング
		};

		/// <summary>
		/// インスタンス生成
		/// </summary>
		/// <param name="shakeProp">シェイク情報</param>
		/// <returns>CameraShakeのインスタンス</returns>
		static std::unique_ptr<CameraShake> Create(const Prop& shakeProp);

		/// <summary>
		/// シェイクの範囲を再設定
		/// </summary>
		/// <param name="shakeRange_">シェイクの範囲</param>
		void SetShakeRange(const Vector3& shakeRange_);

		// シェイク開始
		void Start();

		/// <summary>
		/// 更新処理
		/// </summary>
		/// <returns>シェイクの値</returns>
		Vector3 Update();
	};
}