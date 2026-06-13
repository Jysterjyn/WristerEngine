#include "Collider2D.h"
#include "CollisionManager2D.h"
#include <cassert>
using namespace WE;
using namespace _2D;

//Collider::~Collider()
//{
//	colliders.clear();
//	collisionManager->PopCollider(this);
//}

void Base2DCollider::Initialize(Sprite* transform_, CollisionShapeType shapeType_, const std::string& colliderName_)
{
	transform = transform_;
	shapeType = shapeType_;
	colliderName = colliderName_;
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

bool TestCheckAllCircleCollision::Check2Circles(TestCircleCollider* a, TestCircleCollider* b)
{
	// 値の取得
	Vector2 centerA = a->GetTransform()->position;
	Vector2 centerB = b->GetTransform()->position;
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

void TestCheckAllCircleCollision::CheckCircleCollisions()
{
	auto itrA = colliders.begin();
	for (; itrA != colliders.end(); itrA++)
	{
		auto itrB = itrA;
		itrB++;
		for (; itrB != colliders.end(); itrB++)
		{
			if (!CollisionManager2D::GetInstance()->CheckFiltering(*itrA, *itrB)) { continue; }
			if (Check2Circles(*itrA, *itrB))
			{
				(*itrA)->OnCollision(*itrB);
				(*itrB)->OnCollision(*itrA);
			}
		}
	}
}
