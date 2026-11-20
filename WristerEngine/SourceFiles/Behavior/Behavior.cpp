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
	// クールタイム中でなければ、現在のビヘイビアを更新
	if (!isCoolTime)
	{
		const BehaviorPair& now = behaviorList.front();
		now.first->Update();
		// ビヘイビアが終了していれば、クールタイムを開始
		if (now.first->IsFinish())
		{
			isCoolTime = true;
			coolTime = now.second;
		}
	}
	// クールタイム中であれば、クールタイムを更新
	if (isCoolTime && coolTime.Update())
	{
		// クールタイム終了
		isCoolTime = false;
		// 次のビヘイビアに切り替え
		if (behaviorList.size() > 1)
		{
			auto itr1 = behaviorList.begin();
			auto itr2 = itr1;
			itr2++;
			Behavior::Inherit(itr1->first.get(), itr2->first.get());
		}
		// 現在のビヘイビアを削除
		behaviorList.pop_front();
	}
}

BehaviorParameter* BehaviorPattern::GetParam()
{
	if (behaviorList.empty()) { return nullptr; }
	return behaviorList.front().first->GetParam();
}