#pragma once
#include "Timer.h"
#include <thread>
#include <deque>

namespace WristerEngine
{
	// FPS関連処理
	class FPS final
	{
	private:
		// μを打ち消す変数
		const float MEGA = 1000000.0f;
		std::chrono::steady_clock::time_point reference;
		float fps = 60;
		// FPS測定のインターバル
		RealTimer interval = 0.25f;
		// 1フレームの実行時間記録用配列
		std::deque<float> frameTimes;

		FPS() = default;
		~FPS() = default;

	public:
		FPS(const FPS& obj) = delete;
		FPS& operator=(const FPS& obj) = delete;

		// インスタンス取得
		static FPS* GetInstance();
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="maxfps">最大FPS値</param>
		void Initialize();
		// FPS固定
		void Update();
		// FPS計算
		void CalculateFPS();
		// 前回記録からの経過時間を取得する
		std::chrono::microseconds GetElapsed() const;
		// FPS取得
		float GetFPS() const { return fps; }
	};
}