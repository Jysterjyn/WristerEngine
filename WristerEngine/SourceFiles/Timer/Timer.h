#pragma once
#include <chrono>
#include <array>
#include <WristerEngineUtility.h>

namespace WristerEngine
{
	// タイマーのインターフェースクラス
	class ITimer
	{
	public:
		virtual ~ITimer() = default;
		virtual bool Update() = 0;
		virtual float GetTimeRate() const = 0;
		virtual float GetRemainTimeRate() const = 0;
	};

	// フレーム単位でのタイマー
	class FrameTimer : public ITimer
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
		bool Update() override;
		// 経過フレームを取得
		int GetTime() const { return timer; }
		// 経過フレームを秒に変換して取得
		std::array<int, 2> GetTime(int fps) const { return ConvertToSecond(timer, fps); }
		// 経過フレームを正規化して取得
		float GetTimeRate() const override { return (float)timer / (float)timeMem; }
		// 残りフレームを取得
		int GetRemainTime() const { return timeMem - timer; }
		// 残りフレームを秒に変換して取得
		std::array<int, 2> GetRemainTime(int fps) const { return ConvertToSecond(GetRemainTime(), fps); }
		// 残りフレームを正規化して取得
		float GetRemainTimeRate() const override { return (float)GetRemainTime() / (float)timeMem; }
		// ラップフレームを取得
		int GetInterval() const { return timeMem; }
	};

	// 現実時間でのタイマー
	class RealTimer : public ITimer
	{
	private:
		std::chrono::steady_clock::time_point startTime;
		std::chrono::steady_clock::time_point nowTime;
		float timeMem;

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="interval">制限時間（秒）</param>
		RealTimer(float interval = 0);
		// 更新
		bool Update() override;
		// 計測開始
		void Start() { startTime = std::chrono::steady_clock::now(); }
		// 経過時間を取得
		float GetTime() const;
		// 経過秒数を取得
		float GetSeconds() const { return GetTime() / (float)std::mega::num; }
		// 経過時間を正規化して取得
		float GetTimeRate() const override { return GetTime() / timeMem; }
		// 残り時間を取得
		float GetRemainTime() const { return timeMem - GetTime(); }
		// 残り秒数を取得
		float GetRemainSeconds() const { return GetRemainTime() / (float)std::mega::num; }
		// 残り時間を正規化して取得
		float GetRemainTimeRate() const override { return GetRemainTime() / timeMem; }
		// ラップ時間を取得
		float GetInterval() const { return timeMem; }
	};

	// FrameTimerとRealTimerを区別なく使えるTimerクラス
	class Timer
	{
	private:
		uPtr<ITimer> pTimer;

	public:
		// 初期化
		void operator=(CR<IntOrFloat> interval);
		// 更新
		bool Update() { return pTimer->Update(); }
		// 経過時間を正規化して取得
		float GetTimeRate() const { return pTimer->GetTimeRate(); }
		// 残り時間を正規化して取得
		float GetRemainTimeRate() const { return pTimer->GetRemainTimeRate(); }
		// FrameTimerにキャスト
		FrameTimer* ToFrameTimer() { return dynamic_cast<FrameTimer*>(pTimer.get()); }
		// RealTimerにキャスト
		RealTimer* ToRealTimer() { return dynamic_cast<RealTimer*>(pTimer.get()); }
	};
}