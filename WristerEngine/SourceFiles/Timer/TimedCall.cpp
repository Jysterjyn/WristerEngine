#include "TimedCall.h"

WristerEngine::TimedCall::TimedCall(std::function<void(void)> callfunc_, uint32_t time)
{
	callfunc = callfunc_;
	timer = time;
}

void WristerEngine::TimedCall::Update()
{
	if (isFinished) { return; }
	if (timer.Update())
	{
		isFinished = true;
		// コールバック関数の呼び出し
		callfunc();
	}
}
