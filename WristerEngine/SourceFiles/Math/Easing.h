#pragma once
#include "Timer.h"

namespace WristerEngine
{
	// イージング
	// 参考 : https://easings.net/ja#
	class Easing
	{
	public:
		enum class Type
		{
			Linear,
			Sqrt, // √x
			OutElastic,
			OutBounce,
			EaseInOutBack,
			EaseOutQuint,
			EaseInOutQuint,
		};

		// イージングの最大値
		static const float MAX;

	private:
		FrameTimer timer = 0; // タイマー
		float x = 0; // イージングの割合
		Type type = Type::Sqrt;
		bool isFinish = false;
		bool isLoop = false;
		bool isPostLoop = false;
		FrameTimer loopInterval;

		// イージングの関数テーブル
		static float (Easing::* Ease[])();

		// 線形
		float Linear() { return x; }
		// √x
		float Sqrt() { return std::sqrtf(x); }
		// https://easings.net/ja#easeOutElastic
		float OutElastic();
		// https://easings.net/ja#easeOutBounce
		float OutBounce();
		// https://easings.net/ja#easeInOutBack
		float EaseInOutBack();
		// https://easings.net/ja#easeOutQuint
		float EaseOutQuint() { return 1 - powf(1 - x, 5); }
		// https://easings.net/ja#easeInOutQuint
		float EaseInOutQuint();

	public:
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="easeTime">イージングの時間</param>
		void Initialize(int easeTime, Type type_);

		/// <summary>
		/// 更新
		/// </summary>
		/// <returns>イージングの値(0~1)</returns>
		float Update();

		// リスタート
		void Restart() { timer = timer.GetInterval(); isFinish = false; }

		// 終了したか
		bool IsFinish() const { return isFinish; }

		/// <summary>
		/// ループを設定
		/// </summary>
		/// <param name="loopInterval">ループ開始までの間隔</param>
		void SetLoop(int loopInterval);
	};

	// ループするイージング
	class LoopEasing
	{
	public:
		enum class Type
		{
			Cos,	// cos(x)
		};

	private:
		FrameTimer timer = 0; // タイマー
		float x = 0; // イージングの割合
		Type type;

		// イージングの関数テーブル
		static float (LoopEasing::* Ease[])();

		// cosine
		float Cos();

	public:
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="easeTime">イージングの時間</param>
		void Initialize(int easeTime, Type type_);

		/// <summary>
		/// 更新
		/// </summary>
		/// <returns>イージングの値(0~1)</returns>
		float Update();

		// リスタート
		void Restart() { timer = timer.GetInterval(); }

	};
}