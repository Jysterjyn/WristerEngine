#include "ColliderBase.h"
#include "CollisionManager2D.h"
#include <thread>

using namespace WE;

ColliderInfo::ColliderInfo(uint32_t attribute_, uint32_t mask_)
{
	attribute = attribute_;
	mask = mask_;
}

ColliderInfo::ColliderInfo(const ColliderInfo& info)
{
	attribute = info.attribute;
	mask = info.mask;
	name = info.name.Get();
}

void ColliderInfo::SetColliderInfo(const ColliderInfo& info)
{
	attribute = info.attribute;
	mask = info.mask;
	name = info.name.Get();
}

ColliderInfo ColliderInfo::GetColliderInfo() const
{
	ColliderInfo info{};
	info.attribute = attribute;
	info.mask = mask;
	return info;
}

bool WE::CheckFiltering(const ColliderInfo* infoA, const ColliderInfo* infoB)
{
	return
		infoA->GetAttribute() & infoB->GetMask() &&
		infoB->GetAttribute() & infoA->GetMask();
}

bool WE::CheckCollisionPair(const BaseCollisionPair& p1, const BaseCollisionPair& p2)
{
	// ペアが同じかは、両方のコライダーのシリアルナンバーが同じか、
	// 片方のコライダーのシリアルナンバーがもう片方のコライダーの
	// シリアルナンバーと同じかで判断する
	std::vector<uint32_t> serials1{ p1.my->GetSerialNumber(), p1.other->GetSerialNumber() };
	std::vector<uint32_t> serials2{ p2.my->GetSerialNumber(), p2.other->GetSerialNumber() };

	if (CompareVectors<uint32_t>(serials1, serials2)) { return true; }

	return false;
}

BaseColliderGroup::~BaseColliderGroup()
{
	colliders.clear();
	for (auto* owner : owners) { owner->DeleteGroup(); }
}

BaseSingleCollider* BaseColliderGroup::AddCollider(std::unique_ptr<BaseSingleCollider> newCollider)
{
	colliders.push_back(std::move(newCollider));
	return colliders.back().get();
}

void BaseColliderGroup::Update()
{
	// コライダーの削除
	colliders.remove_if([](std::unique_ptr<BaseSingleCollider>& collider)
		{ return collider.get()->IsDestroy(); });

	for (auto& collider : colliders) { collider->Update(); }
	collisionPairs.clear();
	enterPairs.clear();
	exitPairs.clear();
}

void BaseColliderGroup::CallCollisions()
{
	std::thread enterThread(&BaseColliderGroup::CallEnter, this);
	std::thread exitThread(&BaseColliderGroup::CallExit, this);
	CallCallbacks(collisionPairs, &BaseCollider::OnCollision);
	enterThread.join();
	exitThread.join();
	collisionPairsPre = collisionPairs;
}

void BaseColliderGroup::CallCallbacks(CR<CollisionPairList> pairs, void (BaseCollider::* callback)())
{
	std::map<uint32_t, uint8_t> called;

	for (const auto& pair : pairs)
	{
		BaseCollider* owner = pair.my->GetOwner();
		uint32_t serial = owner->GetSerialNumber();
		// すでに呼ばれているオーナーはスキップ
		if (called.contains(serial)) { continue; }
		(owner->*callback)();
		// 呼び出したオーナーを記録
		called[serial];
	}
}

void BaseColliderGroup::OneCallPair(CollisionPairList& at, CR<CollisionPairList> pair1, CR<CollisionPairList> pair2)
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

void BaseColliderGroup::CallEnter()
{
	OneCallPair(enterPairs, collisionPairs, collisionPairsPre);
	CallCallbacks(enterPairs, &BaseCollider::OnCollisionEnter);
}

void BaseColliderGroup::CallExit()
{
	OneCallPair(exitPairs, collisionPairsPre, collisionPairs);
	CallCallbacks(exitPairs, &BaseCollider::OnCollisionExit);
}

void BaseCollider::Initialize(const std::string& groupName, const std::optional<ColliderInfo>& info)
{
	group = _2D::CollisionManager::GetInstance()->AddGroup(groupName, info);
	group->AddOwner(this);
	if (!info) { return; }
	group->SetColliderInfo(*info);
}

BaseCollider::~BaseCollider()
{
	if (!group) { return; }
	for (auto& collider : *group->GetColliders())
	{
		if (collider->GetOwner() == this) { collider->Destroy(); }
	}
}
