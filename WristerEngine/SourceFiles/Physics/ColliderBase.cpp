#include "ColliderBase.h"
#include "ColliderManager.h"
#include <thread>
#include <unordered_set>

using namespace WE;

uint32_t BaseCollider::nextSerialNumber = 0;
uint32_t BaseSingleCollider::nextSerialNumber = 0;

ColliderInfo::ColliderInfo(uint32_t attribute_, uint32_t mask_, CR<std::string> name_)
{
	attribute = attribute_;
	mask = mask_;
	name = name_;
}

void ColliderInfo::SetColliderInfo(CR<ColliderInfo> info)
{
	attribute = info.attribute;
	mask = info.mask;
	name = info.name;
}

CR<ColliderInfo> ColliderInfo::GetColliderInfo() const
{
	return *this;
}

ColliderGroup::~ColliderGroup()
{
	colliders.clear();
	owner->DeleteGroup();
}

BaseSingleCollider* ColliderGroup::AddCollider(uint32_t colliderShapeType, std::optional<ColliderInfo> info)
{
	uPtr<BaseSingleCollider> newCollider = factory->CreateCollider(colliderShapeType);
	if (info) { newCollider->SetColliderInfo(*info); }
	else 
	{
		ColliderInfo colliderInfo(GetColliderInfo());
		colliderInfo.SetName(GetName());
		newCollider->SetColliderInfo(colliderInfo);
	}
	return static_cast<BaseSingleCollider*>(colliders.Add(std::move(newCollider)));
}

void ColliderGroup::Initialize(IColliderFactory* factory_, BaseCollider* owner_, std::optional<ColliderInfo> info)
{
	SetFactory(factory_);
	SetOwner(owner_);
	if (info) { SetColliderInfo(*info); }
}

void ColliderGroup::Update()
{
	// コライダーの削除
	collisionPairsPre.remove_if([](CR<CollisionPair> pair)
		{ return pair.my->IsDestroy() || pair.other->IsDestroy(); });
	colliders.Update();
	collisionPairs.clear();
	enterPairs.clear();
	exitPairs.clear();
}

void ColliderGroup::CallCollisions()
{
	std::thread enterThread(&ColliderGroup::CallEnter, this);
	std::thread exitThread(&ColliderGroup::CallExit, this);
	CallCallbacks(collisionPairs, &BaseCollider::OnCollision);
	enterThread.join();
	exitThread.join();
	collisionPairsPre = collisionPairs;
}

void ColliderGroup::CallCallbacks(CR<CollisionPairList> pairs, void (BaseCollider::* callback)())
{
	std::unordered_set<uint32_t> called;

	for (const auto& pair : pairs)
	{
		BaseCollider* owner_ = pair.my->GetOwner();
		uint32_t serial = owner_->GetSerialNumber();
		// すでに呼ばれているオーナーはスキップ
		if (called.contains(serial)) { continue; }

		(owner_->*callback)();
		// 呼び出したオーナーを記録
		called.insert(serial);
	}
}

void ColliderGroup::OneCallPair(CollisionPairList& at, CR<CollisionPairList> pair1, CR<CollisionPairList> pair2)
{
	for (const auto& pair : pair1)
	{
		bool isNotEnter = false;
		for (const auto& pairPre : pair2)
		{
			if (CheckCollisionPair(pair, pairPre)) { isNotEnter = true; break; }
		}
		if (isNotEnter) { continue; }
		at.push_back(pair);
	}
}

void ColliderGroup::CallEnter()
{
	OneCallPair(enterPairs, collisionPairs, collisionPairsPre);
	CallCallbacks(enterPairs, &BaseCollider::OnCollisionEnter);
}

void ColliderGroup::CallExit()
{
	OneCallPair(exitPairs, collisionPairsPre, collisionPairs);
	CallCallbacks(exitPairs, &BaseCollider::OnCollisionExit);
}

BaseCollider::~BaseCollider()
{
	if (!group) { return; }
	for (auto& collider : *group->GetColliders())
	{
		BaseSingleCollider* col = static_cast<BaseSingleCollider*>(collider.get());
		if (col->GetOwner() == this) { col->Destroy(); }
	}
}


bool WristerEngine::CheckFiltering(const ColliderInfo* infoA, const ColliderInfo* infoB)
{
	bool isA = infoA->GetAttribute() & infoB->GetMask();
	bool isB = infoB->GetAttribute() & infoA->GetMask();
	bool isAB = isA && isB;
	if (!isAB) { return false; }
	return true;
}

bool WristerEngine::CheckCollisionPair(const CollisionPair& p1, const CollisionPair& p2)
{
	// ペアが同じかは、両方のコライダーのシリアルナンバーが同じか、
	// 片方のコライダーのシリアルナンバーがもう片方のコライダーの
	// シリアルナンバーと同じかで判断する
	std::vector<uint32_t> serials1{ p1.my->GetSerialNumber(), p1.other->GetSerialNumber() };
	std::vector<uint32_t> serials2{ p2.my->GetSerialNumber(), p2.other->GetSerialNumber() };

	if (CompareVectors<uint32_t>(serials1, serials2)) { return true; }

	return false;
}
