#include "Collider2D.h"
#include "CollisionManager2D.h"
#include <cassert>
using namespace WE;
using namespace _2D;

CollisionPair::CollisionPair(BaseCollider* my_, BaseCollider* other_, const HitInfo& hitInfo)
{
	my = my_; other = other_; inter = hitInfo.inter; distance = hitInfo.distance; reject = hitInfo.reject;
}

bool CollisionPair::Check(const CollisionPair& p1, const CollisionPair& p2)
{
	// ペアが同じかは、両方のコライダーのシリアルナンバーが同じか、
	// 片方のコライダーのシリアルナンバーがもう片方のコライダーの
	// シリアルナンバーと同じかで判断する
	if (p1.my->GetSerialNumber() == p2.my->GetSerialNumber() &&
		p1.other->GetSerialNumber() == p2.other->GetSerialNumber())
	{
		return true;
	}

	if (p1.my->GetSerialNumber() == p2.other->GetSerialNumber() &&
		p1.other->GetSerialNumber() == p2.my->GetSerialNumber())
	{
		return true;
	}
	return false;
}

BaseCollider* ColliderGroup::AddCollider(std::unique_ptr<BaseCollider> newCollider)
{
	colliders.push_back(std::move(newCollider));
	return colliders.back().get();
}

void ColliderGroup::Update()
{
	// コライダーの削除
	colliders.remove_if([](std::unique_ptr<BaseCollider>& collider)
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
		Collider* owner = pair.my->GetOwner();
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
		Collider* owner = pair.my->GetOwner();
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
		Collider* owner = pair.my->GetOwner();
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

void Collider::Initialize(const std::string& groupName, const std::optional<CollisionInfo>& info)
{
	group = CollisionManager::GetInstance()->AddGroup(groupName);
	group->AddOwner(this);
	if (!info) { return; }
	group->SetAttribute(info->GetAttribute());
	group->SetMask(info->GetMask());
}

Collider::~Collider()
{
	if (!group) { return; }
	for (auto& collider : *group->GetColliders())
	{
		if (collider->GetOwner() == this) { collider->Destroy(); }
	}
}

static bool Check2Circles(const CircleCollider* a, const CircleCollider* b)
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
//
//void Collider::AddCollider(Sprite* transform, CollisionShapeType shapeType, const std::string& colliderName, const Option* option)
//{
//	std::unique_ptr<Base2DCollider> newCollider;
//	switch (shapeType)
//	{
//	case CollisionShapeType::Box:
//		newCollider = std::make_unique<BoxCollider>();
//		break;
//	case CollisionShapeType::TwoRay:
//		assert(option);
//		newCollider = std::make_unique<TwoRayCollider>(option->fov);
//		break;
//	default:
//		break;
//	}
//
//	newCollider->Initialize(transform, shapeType, colliderName);
//	colliders.push_back(move(newCollider));
//}
//
//void Collider::DeleteCollider(const std::string& colliderName)
//{
//	colliders.remove_if([&](const std::unique_ptr<Base2DCollider>& collider) { return collider->GetColliderName() == colliderName; });
//}
//
//void Collider::AddCollisionPair(size_t myIndex, size_t youIndex)
//{
//	collisionPair[myIndex].push_back(youIndex);
//}
//
//void Collider::DeletePair()
//{
//	for (auto& pair : collisionPair)
//	{
//		pair.second.clear();
//	}
//	collisionPair.clear();
//}
//
//const std::string Collider::GetColliderName(size_t index) const
//{
//	if (colliders.size() <= index) { return "Null"; }
//	auto itr = colliders.begin();
//	for (size_t i = 0; i < index; i++)
//	{
//		itr++;
//	}
//	return itr->get()->GetColliderName();
//}
// 
//void TestCheckAllCircleCollision::CheckCircleCollisions()
//{
//	auto itrA = colliders.begin();
//	for (; itrA != colliders.end(); itrA++)
//	{
//		auto itrB = itrA;
//		itrB++;
//		for (; itrB != colliders.end(); itrB++)
//		{
//			if (!CollisionManager2D::GetInstance()->CheckFiltering(*itrA, *itrB)) { continue; }
//			if (Check2Circles(*itrA, *itrB))
//			{
//				(*itrA)->OnCollision(*itrB);
//				(*itrB)->OnCollision(*itrA);
//			}
//		}
//	}
//}
