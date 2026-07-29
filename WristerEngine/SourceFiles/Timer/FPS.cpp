#include "FPS.h"
#include <numeric>
#include <MathUtility.h>
using namespace std::chrono;
using namespace WE;

FPS* FPS::GetInstance()
{
	static FPS instance;
	return &instance;
}

void FPS::Initialize()
{
	reference = steady_clock::now();
}

void FPS::Update()
{
	// 1/MAX_FPS 秒ぴったりの時間
	const microseconds MIN_TIME(uint64_t(MEGA / MAX_FPS));
	// 1/MAX_FPS 秒よりわずかに短い時間
	const float CHECK_TIME_OFFSET = 5;
	const microseconds MIN_CHECK_TIME(uint64_t(MEGA / (MAX_FPS + CHECK_TIME_OFFSET)));

	// 1/MAX_FPS 秒(よりわずかに短い時間)経っていない場合
	if (GetElapsed() < MIN_CHECK_TIME)
	{
		// 1/MAX_FPS 秒経過するまで微小なスリープを繰り返す
		while (GetElapsed() < MIN_TIME)
		{
			// 1マイクロ秒スリープ
			std::this_thread::sleep_for(microseconds(1));
		}
	}

	// FPSを計算する
	CalculateFPS();

	// 現在の時間を記録する
	reference = steady_clock::now();
}

void WE::FPS::CalculateFPS()
{
	if (isFirstFrame)
	{
		isFirstFrame = false;
		return;
	}

	frameTimes.push_back((float)GetElapsed().count());
	// 60フレーム以上のデータが溜まったら古いデータを削除する
	while (frameTimes.size() > 60) { frameTimes.pop_front(); }
	// 60フレーム分の平均FPSを計算する
	if (interval.Update()) { fps = MEGA / Average(frameTimes); }
}

microseconds WE::FPS::GetElapsed() const
{
	return  duration_cast<microseconds>(steady_clock::now() - reference);
}