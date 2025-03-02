#pragma once
#include <chrono>
#include <array>

namespace WristerEngine
{
	// フレーム単位でのタイマー
	class FrameTimer
	{
	private:
		int timer;
		int timeMem;

		/// <summary>
		/// フレーム数を秒に変換
		/// </summary>
		/// <param name="fps">今のFPS</param>
		/// <returns>[0]に秒、[1]に999以下のミリ秒が入る</returns>
		std::array<int, 2> ConvertToSecond(int time, int fps) const;

	public:
		// コンストラクタ
		FrameTimer(int interval = 0) { timer = 0; timeMem = interval; }
		// 更新
		bool Update();
		// 経過フレームを取得
		int GetTime() const { return timer; }
		// 経過フレームを秒に変換して取得
		std::array<int, 2> GetTime(int fps) const { return ConvertToSecond(timer, fps); }
		// 経過フレームを正規化して取得
		float GetTimeRate() const { return (float)timer / (float)timeMem; }
		// 残りフレームを取得
		int GetRemainTime() const { return timeMem - timer; }
		// 残りフレームを秒に変換して取得
		std::array<int, 2> GetRemainTime(int fps) const { return ConvertToSecond(GetRemainTime(), fps); }
		// 残りフレームを正規化して取得
		float GetRemainTimeRate() const { return (float)GetRemainTime() / (float)timeMem; }
		// ラップフレームを取得
		int GetInterval() const { return timeMem; }
	};

	// 現実時間でのタイマー
	class RealTimer
	{
	private:
		std::chrono::steady_clock::time_point startTime;
		std::chrono::steady_clock::time_point nowTime;
		float timeMem;

	public:
		// コンストラクタ
		RealTimer(float limitTime = 0) { timeMem = limitTime; startTime = std::chrono::steady_clock::now(); }
		// 更新
		bool Update();
		// 計測開始
		void Start() { startTime = std::chrono::steady_clock::now(); }
		// 経過時間を取得
		float GetTime() const;
		// 経過時間を正規化して取得
		float GetTimeRate() const { return GetTime() / timeMem; }
		// 残り時間を取得
		float GetRemainTime() const { return timeMem - GetTime(); }
		// 残り時間を正規化して取得
		float GetRemainTimeRate() const { return GetRemainTime() / timeMem; }
		// ラップ時間を取得
		float GetInterval() const { return timeMem; }
	};
}