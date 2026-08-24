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
	// FPSを計算する
	CalculateFPS();

	// 現在の時間を記録する
	reference = steady_clock::now();
}

void WE::FPS::CalculateFPS()
{
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