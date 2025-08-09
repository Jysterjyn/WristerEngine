#include "Behavior.h"
using namespace WE;

void Behavior::Inherit(Behavior* b1, Behavior* b2)
{
	b2->param = std::move(b1->param);
	b2->Initialize();
}

void BehaviorPattern::Add(BehaviorPair& behavior)
{
	behaviorList.push_back(std::move(behavior));
}

void BehaviorPattern::Update()
{
	if (behaviorList.empty()) { return; }
	if (!isCoolTime)
	{
		const BehaviorPair& now = behaviorList.front();
		now.first->Update();
		if (now.first->IsFinish())
		{
			isCoolTime = true;
			coolTime = now.second;
		}
	}
	if (isCoolTime)
	{
		if (coolTime.Update())
		{
			isCoolTime = false;
			if(behaviorList.size() > 1)
			{
				auto itr1 = behaviorList.begin();
				auto itr2 = itr1;
				itr2++;
				Behavior::Inherit(itr1->first.get(), itr2->first.get());
			}
			behaviorList.pop_front();
		}
	}
}

const Behavior::Parameter* WristerEngine::BehaviorPattern::GetParam() const
{
	if(behaviorList.empty()){ return nullptr; }
	return behaviorList.front().first->GetParam();
}