#include "Easing.h"
#include "MathUtility.h"

using namespace WristerEngine;

const float Easing::MAX = 1.0f;

float(Easing::* Easing::Ease[])() =
{
	&Easing::Linear,
	&Easing::Sqrt,
	&Easing::OutElastic,
	&Easing::OutBounce,
	&Easing::EaseInOutBack,
	&Easing::EaseOutQuint,
	&Easing::EaseInOutQuint,
};

float(LoopEasing::* LoopEasing::Ease[])() =
{
	&LoopEasing::Cos,
};

float Easing::OutElastic()
{
	const float c4 = (2.0f * PI) / 3.0f;

	if (x == 0) { return 0; }
	return powf(2.0f, -10.0f * x) * sinf((x * 10.0f - 0.75f) * c4) + 1.0f;
}

float Easing::OutBounce()
{
	const float n1 = 7.5625f;
	const float d1 = 2.75f;

	if (x < 1 / d1) { return n1 * x * x; }
	else if (x < 2.0f / d1) { return n1 * (x -= 1.5f / d1) * x + 0.75f; }
	else if (x < 2.5f / d1) { return n1 * (x -= 2.25f / d1) * x + 0.9375f; }
	else { return n1 * (x -= 2.625f / d1) * x + 0.984375f; }
}

float Easing::EaseInOutBack()
{
	const float c1 = 1.70158f;
	const float c2 = c1 * 1.525f;

	return x < 0.5f
		? (powf(2 * x, 2) * ((c2 + 1) * 2 * x - c2)) / 2
		: (powf(2 * x - 2, 2) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2;
}

float Easing::EaseInOutQuint()
{
	return x < 0.5f ? 16 * powf(x, 5) : 1 - powf(-2 * x + 2, 5) / 2;
}

void Easing::Initialize(int easeTime, Type type_)
{
	timer = easeTime;
	type = type_;
}

float Easing::Update()
{
	if (isFinish)
	{
		if (isLoop)
		{
			if (loopInterval.Update())
			{
				isFinish = false;
				isPostLoop = !isPostLoop;
				if (isPostLoop) { return Easing::MAX; }
				else { return 0.0f; }
			}
		}
		if (!isPostLoop) { return Easing::MAX; }
		else { return 0.0f; }
	}
	if (!isPostLoop) { x = timer.GetTimeRate(); }
	else { x = timer.GetRemainTimeRate(); }
	float easeNum = (this->*Ease[(int)type])();
	isFinish = timer.Update();
	return easeNum;
}

void WristerEngine::Easing::SetLoop(int loopInterval_)
{
	isLoop = true;
	loopInterval = loopInterval_;
}

float LoopEasing::Cos()
{
	return (cos(2.0f * PI * x) + 1) * 0.5f;
}

void LoopEasing::Initialize(int easeTime, Type type_)
{
	timer = easeTime;
	type = type_;
}

float LoopEasing::Update()
{
	x = timer.GetTimeRate();
	float easeNum = (this->*Ease[(int)type])();
	timer.Update();
	return easeNum;
}
