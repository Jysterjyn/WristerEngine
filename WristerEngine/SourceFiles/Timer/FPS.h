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
		uPtr<std::thread> calculateFPSThread;
		bool isFinish = false;

		FPS() = default;
		~FPS() = default;
		FPS(const FPS& obj) = delete;
		FPS& operator=(const FPS& obj) = delete;

	public:
		// インスタンス取得
		static FPS* GetInstance();
		// 初期化
		void Initialize();
		// 更新
		void Update();
		// FPS計算
		void CalculateFPS();
		// 終了処理
		void Finalize();
		// 前回記録からの経過時間を取得する
		float GetDeltaTime() const;
		// 現在のフレームの経過時間を取得する
		float GetCurrentDeltaTime() const { return frameTimes.empty() ? 0.0f : frameTimes.back(); }
		// FPS取得
		float GetFPS() const { return fps; }
	};
}