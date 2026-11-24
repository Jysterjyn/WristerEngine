#include "Timer.h"
using namespace std::chrono;
using namespace WE;

std::array<int, 2> FrameTimer::ConvertToSecond(int time, int fps) const
{
	std::array<float, 2> timef{};
	// ゼロ除算を回避
	fps = std::max(1, fps);
	// 残り時間を取得
	float t = (float)time / fps;
	// 整数部と小数部を分離
	timef[1] = modf(t, &timef[0]);
	return std::array<int, 2>({ (int)timef[0], (int)(timef[1] * std::kilo::num) });
}

bool FrameTimer::Update()
{
	if (timeMem == 0) { return true; }

	if (++timer >= timeMem)
	{
		timer = 0;
		return true;
	}
	return false;
}

RealTimer::RealTimer(float interval)
{
	timeMem = interval * std::mega::num;
	startTime = steady_clock::now();
}

bool RealTimer::Update()
{
	if (timeMem == 0.0f) { return true; }

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
	return (float)duration_cast<microseconds>(nowTime - startTime).count();
}

void Timer::operator=(CR<IntOrFloat> interval)
{
	if (interval.IsInt()) { pTimer = std::make_unique<FrameTimer>(interval.GetInt()); }
	else if (interval.IsFloat()) { pTimer = std::make_unique<RealTimer>(interval.GetFloat()); }
}
