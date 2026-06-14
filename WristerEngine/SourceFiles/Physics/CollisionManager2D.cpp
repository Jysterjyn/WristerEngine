#include "CollisionManager2D.h"
using namespace WE;
using namespace _2D;

CollisionManager* CollisionManager::GetInstance()
{
	static CollisionManager instance;
	return &instance;
}

ColliderGroup* CollisionManager::AddGroup(const std::string& groupName)
{
	if (!colliderGroups.contains(groupName))
	{
		std::unique_ptr<ColliderGroup> newGroup = std::make_unique<ColliderGroup>();
		colliderGroups[groupName] = std::move(newGroup);
	}
	return colliderGroups[groupName].get();
}

void CollisionManager::CheckCollisions()
{
	for (auto& colliderGroup : colliderGroups) { colliderGroup.second->Update(); }

	auto itrA = colliderGroups.begin();
	for (; itrA != colliderGroups.end(); itrA++)
	{
		auto itrB = itrA;
		itrB++;
		for (; itrB != colliderGroups.end(); itrB++)
		{
			ColliderGroup* groupA = itrA->second.get();
			ColliderGroup* groupB = itrB->second.get();
			if (!CheckFiltering(groupA, groupB)) { continue; }
			if (!Check2Groups(groupA, groupB)) { continue; }
		}
	}

	for (auto& group : colliderGroups)
	{
		group.second->CallCollision();
	}
}

bool CollisionManager::Check2Groups(ColliderGroup* groupA, ColliderGroup* groupB)
{
	for (const auto& colliderA : *groupA->GetColliders()) {
		for (const auto& colliderB : *groupB->GetColliders())
		{
			if (!CheckFiltering(colliderA.get(), colliderB.get())) { continue; }

			std::list<BaseCollider*> colliderPair({ colliderA.get(),colliderB.get() });
			colliderPair.sort([](BaseCollider* c1, BaseCollider* c2)
				{
					return c1->GetShapeType() < c2->GetShapeType();
				});

			if (Check2Collisions(colliderPair.front(), colliderPair.back()))
			{
				CollisionPair pairA(colliderA.get(), colliderB.get(), *this);
				CollisionPair pairB(colliderB.get(), colliderA.get(), *this);
				groupA->AddCollisionPair(pairA);
				groupB->AddCollisionPair(pairB);
				Reset();
			}
		}
	}

	return !groupA->GetCollisionPairs().empty();
}

//bool CollisionManager::Check2DCollision2Boxes(const std::array<_2D::Base2DCollider*, 2>& colliders)
//{
//	std::array<const _2D::BoxCollider*, 2> box2DColliders{};
//	for (size_t i = 0; i < colliders.size(); i++)
//	{
//		box2DColliders[i] = dynamic_cast<_2D::BoxCollider*>(colliders[i]);
//	}
//
//	std::array<Vector2, 2> posCenter;
//	std::array<const _2D::Sprite*, 2> trans{};
//
//	for (size_t i = 0; i < box2DColliders.size(); i++)
//	{
//		trans[i] = box2DColliders[i]->GetTransform();
//		// 中心点を計算
//		auto pos = box2DColliders[i]->GetVertex();
//		posCenter[i] = Half<Vector2>(pos["LT"] + pos["RB"]);
//	}
//
//	// 当たり判定
//	if (std::abs(posCenter[0].x - posCenter[1].x) <= Half(trans[0]->size.x + trans[1]->size.x) &&
//		std::abs(posCenter[0].y - posCenter[1].y) <= Half(trans[0]->size.y + trans[1]->size.y))
//	{
//		return true;
//	}
//	return false;
//}
//
//bool CollisionManager::Check2DCollisionBox2Rays(const std::array<_2D::Base2DCollider*, 2>& colliders)
//{
//	const _2D::TwoRayCollider* rayCollider = nullptr;
//	const _2D::BoxCollider* boxCollider = nullptr;
//	for (auto collider : colliders)
//	{
//		if (collider->GetShapeType() == _2D::CollisionShapeType::Box)
//		{
//			boxCollider = dynamic_cast<_2D::BoxCollider*>(collider);
//		}
//		if (collider->GetShapeType() == _2D::CollisionShapeType::TwoRay)
//		{
//			rayCollider = dynamic_cast<_2D::TwoRayCollider*>(collider);
//		}
//	}
//
//	const _2D::Sprite* rayTrans = rayCollider->GetTransform();
//
//	// 視野角を計算
//	float leftRot = Angle(90) + rayCollider->GetFOV() + rayTrans->rotation;
//	float rightRot = Angle(90) - rayCollider->GetFOV() + rayTrans->rotation;
//
//	// ボックスの左上端と右下端の座標を求める
//	const _2D::Sprite* pSprite = boxCollider->GetTransform();
//	Vector2 pPosLT, pPosRB;
//	pPosLT = pPosRB = pSprite->position;
//
//	pPosLT -= Vector2(pSprite->size.x * pSprite->anchorPoint.x, pSprite->size.y * pSprite->anchorPoint.y);
//	pPosRB += Vector2(pSprite->size.x * (1.0f - pSprite->anchorPoint.x), pSprite->size.y * (1.0f - pSprite->anchorPoint.y));
//
//	// 左上の接触判定
//	Vector2 vec = Normalize(Vector2(std::cos(rightRot), std::sin(rightRot)));
//	Vector2 toEyePlayerLT = Normalize(pPosLT - rayTrans->position);
//	float crossLT = Cross(vec, Normalize(toEyePlayerLT));
//
//	// 右下の接触判定
//	vec = Normalize(Vector2(std::cos(leftRot), std::sin(leftRot)));
//	Vector2 toEyePlayerRB = Normalize(pPosRB - rayTrans->position);
//	float crossRB = Cross(vec, Normalize(toEyePlayerRB));
//
//	if (crossRB < 0 && crossLT > 0) { return true; }
//
//	Vector2 pPosLB, pPosRT;
//	pPosLB = pPosRT = pSprite->position;
//
//	pPosLB += Vector2(-pSprite->size.x * pSprite->anchorPoint.x, pSprite->size.y * (1.0f - pSprite->anchorPoint.y));
//	pPosRT += Vector2(pSprite->size.x * (1.0f - pSprite->anchorPoint.x), -pSprite->size.y * pSprite->anchorPoint.y);
//
//	// 左下の接触判定
//	vec = Normalize(Vector2(std::cos(rightRot), std::sin(rightRot)));
//	Vector2 toEyePlayerLB = Normalize(pPosLB - rayTrans->position);
//	float crossLB = Cross(vec, Normalize(toEyePlayerLB));
//
//	// 右上の接触判定
//	vec = Normalize(Vector2(std::cos(leftRot), std::sin(leftRot)));
//	Vector2 toEyePlayerRT = Normalize(pPosRT - rayTrans->position);
//	float crossRT = Cross(vec, Normalize(toEyePlayerRT));
//
//	return crossRT < 0 && crossLB > 0;
//}
//
//bool CollisionManager::CheckCollisionRayBox(RayCollider* colliderA, BoxCollider* colliderB)
//{
//	if (!CheckFiltering(colliderA, colliderB)) { return false; }
//	PolygonCollider pCollider;
//	Vector3 boxPos = colliderB->GetCenterPosition();
//	Vector3 boxRad = colliderB->GetRadius3D();
//	// 上底左奥
//	pCollider.AddVertices({ boxPos.x - boxRad.x,boxPos.y + boxRad.y,boxPos.z + boxRad.z });
//	// 上底右奥
//	pCollider.AddVertices({ boxPos.x + boxRad.x,boxPos.y + boxRad.y,boxPos.z + boxRad.z });
//	// 上底右前
//	pCollider.AddVertices({ boxPos.x + boxRad.x,boxPos.y + boxRad.y,boxPos.z - boxRad.z });
//	// 上底左前
//	pCollider.AddVertices({ boxPos.x - boxRad.x,boxPos.y + boxRad.y,boxPos.z - boxRad.z });
//
//	pCollider.SetBaseNormal({ Vector3::MakeAxis(Axis::Y) });
//
//	return CheckCollisionRayPolygon(colliderA, &pCollider);
//}
//
//bool CollisionManager::CheckCollision2ColliderGroups(_2D::Collider* groupA, _2D::Collider* groupB)
//{
//	if (!CheckFiltering(groupA, groupB)) { return false; }
//
//	const auto& collidersA = groupA->GetColliders();
//	const auto& collidersB = groupB->GetColliders();
//
//	bool isHitGroup = false;
//	size_t aIndex = 0;
//
//	// 2つのコライダーグループの全てのコライダーと当たり判定を取る
//	std::map<_2D::CollisionShapeType, size_t> shapeTypeCount;
//	auto itrA = collidersA.begin();
//	for (; itrA != collidersA.end(); itrA++, aIndex++)
//	{
//		bool isHitPair = false;
//		auto itrB = collidersB.begin();
//		size_t bIndex = 0;
//		for (; itrB != collidersB.end(); itrB++, bIndex++)
//		{
//			shapeTypeCount.clear();
//			shapeTypeCount[itrA->get()->GetShapeType()]++;
//			shapeTypeCount[itrB->get()->GetShapeType()]++;
//			// ボックスとボックスの当たり判定
//			if (shapeTypeCount[_2D::CollisionShapeType::Box] == 2)
//			{
//				isHitPair = Check2DCollision2Boxes({ itrA->get(),itrB->get() });
//			}
//			// ボックスと2レイの当たり判定
//			else if (shapeTypeCount[_2D::CollisionShapeType::Box] == shapeTypeCount[_2D::CollisionShapeType::TwoRay])
//			{
//				isHitPair = Check2DCollisionBox2Rays({ itrA->get(),itrB->get() });
//			}
//
//			if (isHitPair)
//			{
//				// コリジョンペアの登録
//				groupA->AddCollisionPair(aIndex, bIndex);
//				groupB->AddCollisionPair(bIndex, aIndex);
//				isHitGroup = true;
//			}
//		}
//	}
//
//	return isHitGroup;
//}
//
//void CollisionManager::Check2DCollisions()
//{
//	for (auto& collider : _2DColliders)
//	{
//		collider->DeletePair();
//	}
//
//	auto itrA = _2DColliders.begin();
//	for (; itrA != _2DColliders.end(); itrA++)
//	{
//		list<_2D::Collider*>::iterator itrB = itrA;
//		itrB++;
//		for (; itrB != _2DColliders.end(); itrB++)
//		{
//			if (!CheckCollision2ColliderGroups(*itrA, *itrB)) { continue; }
//
//			(*itrA)->OnCollision(*itrB);
//			(*itrB)->OnCollision(*itrA);
//		}
//	}
//}