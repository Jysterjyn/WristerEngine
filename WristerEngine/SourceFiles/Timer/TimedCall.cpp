#include "TimedCall.h"

WE::TimedCall::TimedCall(std::function<void(void)> callfunc_, CR<IntOrFloat> interval)
{
	callfunc = callfunc_;
	timer = interval;
}

void WE::TimedCall::Update()
{
	if (isFinished) { return; }
	if (timer.Update())
	{
		isFinished = true;
		// コールバック関数の呼び出し
		callfunc();
	}
}
