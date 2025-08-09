#include "Bahavior.h"
using namespace WE;

void Behavior::Inherit(uPtr<Behavior>& b1, uPtr<Behavior>& b2)
{
	b2->param = b1->param;
	b1.release();
}

void BehaviorPattern::Add(uPtr<BehaviorPair>& behavior)
{
	behaviorList.push_back(std::move(behavior));
}

void BehaviorPattern::Update()
{
	if (behaviorList.empty()) { return; }
	if (!isCoolTime)
	{
		uPtr<BehaviorPair>& now = behaviorList.front();
		now->first.Update();
		if (now->first.IsFinish())
		{
			isCoolTime = true;
			coolTime = now->second;
		}
	}
	else
	{
		if (coolTime.Update())
		{
			isCoolTime = false;
			behaviorList.pop_front();
		}
	}
}
