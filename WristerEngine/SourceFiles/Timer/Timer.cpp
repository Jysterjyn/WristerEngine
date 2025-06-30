#include "Timer.h"
using namespace std::chrono;
using namespace WristerEngine;

std::array<int, 2> FrameTimer::ConvertToSecond(int time, int fps) const
{
	std::array<float, 2> timef{};
	// ミリ秒を取得するための変数
	const float KILO = 1000.0f;
	// ゼロ除算を回避
	fps = std::max(1, fps);
	// 残り時間を取得
	float t = (float)time / fps;
	// 整数部と小数部を分離
	timef[1] = modf(t, &timef[0]);
	return std::array<int, 2>({ (int)timef[0], (int)(timef[1] * KILO) });
}

bool FrameTimer::Update()
{
	if (++timer >= timeMem)
	{
		timer = 0;
		return true;
	}
	return false;
}

bool RealTimer::Update()
{
	nowTime = steady_clock::now();
	if (GetTime() >= timeMem)
	{
		startTime = nowTime;
		return true;
	}
	return false;
}

float RealTimer::GetTime() const
{
	// 単位を秒(s)にするための変数
	const float KILO = 1000.0f;
	return (float)duration_cast<milliseconds>(nowTime - startTime).count() / KILO;
}
