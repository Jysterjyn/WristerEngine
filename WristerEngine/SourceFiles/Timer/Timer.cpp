#include "Timer.h"
using namespace std::chrono;
using namespace WristerEngine;

std::array<int, 2> FrameTimer::ConvertToSecond(int time, int fps) const
{
	std::array<float, 2> timef{};
	// ƒ~ƒŠ•b‚ğæ“¾‚·‚é‚½‚ß‚Ì•Ï”
	const float KILO = 1000.0f;
	// ƒ[ƒœZ‚ğ‰ñ”ğ
	fps = std::max(1, fps);
	// c‚èŠÔ‚ğæ“¾
	float t = (float)time / fps;
	// ®”•”‚Æ¬”•”‚ğ•ª—£
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
	// ’PˆÊ‚ğ•b(s)‚É‚·‚é‚½‚ß‚Ì•Ï”
	const float KILO = 1000.0f;
	return (float)duration_cast<milliseconds>(nowTime - startTime).count() / KILO;
}
