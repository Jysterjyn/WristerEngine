#pragma once
#include "Timer.h"

namespace WristerEngine
{
	const int MAX_FPS = 60;

	// FPS関連処理
	class FPS final
	{
	private:
		std::chrono::steady_clock::time_point reference;
		int fps = 0;
		RealTimer oneSecond = 1.0f;
		FrameTimer oneSecondFrames = INT32_MAX;

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
		void Fix();
		// FPS取得
		int GetFPS() const { return fps; }
	};
}