#include "Collider2D.h"
#include "CollisionManager2D.h"
#include <cassert>
using namespace WE;
using namespace _2D;

uint32_t SingleCollider2D::nextSerialNumber = 0;
uint32_t Collider::nextSerialNumber = 0;


SingleCollider2D* ColliderGroup::AddCollider(std::unique_ptr<SingleCollider2D> newCollider)
{
	colliders.push_back(std::move(newCollider));
	return colliders.back().get();
}

void ColliderGroup::Update()
{
	// コライダーの削除
	colliders.remove_if([](std::unique_ptr<SingleCollider2D>& collider)
		{ return collider.get()->IsDestroy(); });

	for (auto& collider : colliders) { collider->Update(); }
	collisionPairs.clear();
	enterPairs.clear();
	exitPairs.clear();
}

void ColliderGroup::AddCollisionPair(const CollisionPair& pair)
{
	collisionPairs.push_back(pair);
}

void ColliderGroup::CallCollision()
{
	// OnCollisionは当たっている間に呼ばれるコールバック関数なので、同じオーナーに対して複数回呼ばれないようにする
	std::map<uint32_t, uint8_t> calledCollision;

	// コールバック関数呼び出し(OnCollision)
	for (auto& pair : collisionPairs)
	{
		BaseCollider* owner = pair.my->GetOwner();
		// すでに呼ばれているオーナーはスキップ
		if (calledCollision.contains(owner->GetSerialNumber())) { continue; }
		owner->OnCollision();
		// 呼び出したオーナーを記録
		calledCollision[owner->GetSerialNumber()];
	}

	// OnCollisionEnterは当たった瞬間に呼ばれるコールバック関数なので、前フレームのペアと同じペアは呼ばない
	for (auto& pair : collisionPairs)
	{
		bool isNotEnter = false;
		for (auto& pairPre : collisionPairsPre)
		{
			if (CollisionPair::Check(pair, pairPre)) { isNotEnter = true; break; }
		}
		if (isNotEnter) { continue; }
		enterPairs.push_back(pair);
	}
	calledCollision.clear();
	for (auto& pair : enterPairs)
	{
		BaseCollider* owner = pair.my->GetOwner();
		if (calledCollision.contains(owner->GetSerialNumber())) { continue; }
		owner->OnCollisionEnter();
		calledCollision[owner->GetSerialNumber()];
	}

	// OnCollisionExitは離れた瞬間に呼ばれるコールバック関数なので、前フレームのペアで今回当たっていないペアだけ呼ぶ
	for (auto& pair : collisionPairsPre)
	{
		bool isNotEnter = false;
		for (auto& pairPre : collisionPairs)
		{
			if (CollisionPair::Check(pair, pairPre)) { isNotEnter = true; break; }
		}
		if (isNotEnter) { continue; }
		exitPairs.push_back(pair);
	}
	calledCollision.clear();
	for (auto& pair : exitPairs)
	{
		BaseCollider* owner = pair.my->GetOwner();
		if (calledCollision.contains(owner->GetSerialNumber())) { continue; }
		owner->OnCollisionExit();
		calledCollision[owner->GetSerialNumber()];
	}

	collisionPairsPre = collisionPairs;
}

ColliderGroup::~ColliderGroup()
{
	colliders.clear();
	for (auto* owner : owners) { owner->DeleteGroup(); }
}

void Collider::Initialize(const std::string& groupName, const std::optional<ColliderInfo>& info)
{
	group = CollisionManager::GetInstance()->AddGroup(groupName, info);
	group->AddOwner(this);
	if (!info) { return; }
	group->SetColliderInfo(*info);
}

Collider::~Collider()
{
	if (!group) { return; }
	for (auto& collider : *group->GetColliders())
	{
		if (collider->GetOwner() == this) { collider->Destroy(); }
	}
}

//std::map<std::string, Vector2> BoxCollider::GetVertex() const
//{
//	std::map<std::string, Vector2> ans;
//	ans["LT"] = ans["RB"] = ans["LB"] = ans["RT"] = transform->position;
//	// 差分
//	Vector2 ltSub = Vector2(transform->size.x * transform->anchorPoint.x, transform->size.y * transform->anchorPoint.y);
//	Vector2 rbSub = Vector2(transform->size.x * (1.0f - transform->anchorPoint.x), transform->size.y * (1.0f - transform->anchorPoint.y));
//	if (transform->isFlipX) { ltSub.x = -ltSub.x; rbSub.x = -rbSub.x; }
//	if (transform->isFlipY) { ltSub.y = -ltSub.y; rbSub.y = -rbSub.y; }
//	ans["LT"] -= ltSub;
//	ans["LB"] += ltSub;
//	ans["RT"] -= rbSub;
//	ans["RB"] += rbSub;
//	return ans;
//}

bool WE::_2D::Check2Circles(const CircleCollider* a, const CircleCollider* b)
{
	// 値の取得
	Vector2 centerA = a->GetCenterPosition();
	Vector2 centerB = b->GetCenterPosition();
	float radA = a->GetRadius();
	float radB = b->GetRadius();

	//判定対象の座標
	Vector2 vecAB = centerA - centerB;
	float dist = Dot(vecAB, vecAB);
	//判定対象の半径
	float radAB = radA + radB;

	if (dist > radAB * radAB) { return false; }
	return true;
}
