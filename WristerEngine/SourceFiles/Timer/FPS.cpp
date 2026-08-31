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
	// FPS計算を別スレッドで行う
	calculateFPSThread = std::make_unique<std::thread>(&FPS::CalculateFPS, this);
}

void FPS::Update()
{
	// 前回のフレームからの経過時間を取得して記録する
	frameTimes.push_back(GetDeltaTime());
	// 現在の時間を記録する
	reference = steady_clock::now();
}

void FPS::CalculateFPS()
{
	while (!isFinish)
	{
		// 60フレーム以上のデータが溜まったら古いデータを削除する
		while (frameTimes.size() > 60) { frameTimes.pop_front(); }
		// 60フレーム分の平均FPSを計算する
		if (interval.Update()) { fps = MEGA / Average(frameTimes); }
	}
}

void FPS::Finalize()
{
	isFinish = true;
	if (calculateFPSThread && calculateFPSThread->joinable()) { calculateFPSThread->join(); }
}

float FPS::GetDeltaTime() const
{
	return (float)(duration_cast<microseconds>(steady_clock::now() - reference)).count();
}