#include "Collider2D.h"
#include "ColliderManager.h"
#include <cassert>
using namespace WE;
using namespace _2D;

static BaseColliderManager* GetColliderManager2D()
{
	return ColliderManager::GetInstance()->GetColliderManager(ColliderDimension::_2D);
}

ColliderGroup* Collider::Initialize(CR<std::string> groupName, const std::optional<ColliderInfo>& info)
{
	group = GetColliderManager2D()->AddGroup(groupName, this, info);
	return group;
}

uPtr<BaseSingleCollider> ColliderFactory::CreateCollider(uint32_t colliderShapeType)
{
	uPtr<BaseSingleCollider> newCollider;

	switch (colliderShapeType)
	{
	case (uint32_t)CollisionShapeType::Circle:
		newCollider = std::make_unique<CircleCollider>();
		break;
	case (uint32_t)CollisionShapeType::Box:
		newCollider = std::make_unique<BoxCollider>();
		break;
	case (uint32_t)CollisionShapeType::TwoRay:
		newCollider = std::make_unique<TwoRayCollider>();
		break;
	default:
		return nullptr;
	}
	return newCollider;
}

//std::map<std::string, Vector2> BoxCollider::GetVertex() const
//{
//	std::map<std::string, Vector2> ans;
//	ans["LT"] = ans["RB"] = ans["LB"] = ans["RT"] = pTransform->position;
//	// 差分
//	Vector2 ltSub = Vector2(pTransform->size.x * pTransform->anchorPoint.x, pTransform->size.y * pTransform->anchorPoint.y);
//	Vector2 rbSub = Vector2(pTransform->size.x * (1.0f - pTransform->anchorPoint.x), pTransform->size.y * (1.0f - pTransform->anchorPoint.y));
//	if (pTransform->isFlipX) { ltSub.x = -ltSub.x; rbSub.x = -rbSub.x; }
//	if (pTransform->isFlipY) { ltSub.y = -ltSub.y; rbSub.y = -rbSub.y; }
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

bool WE::_2D::Check2Boxes(const std::array<_2D::BoxCollider*, 2>& colliders)
{
	colliders;
	std::array<const _2D::BoxCollider*, 2> box2DColliders{};
	for (size_t i = 0; i < colliders.size(); i++)
	{
		box2DColliders[i] = dynamic_cast<_2D::BoxCollider*>(colliders[i]);
	}

	std::array<Vector2, 2> posCenter;
	std::array<const Transform*, 2> trans{};

	for (size_t i = 0; i < box2DColliders.size(); i++)
	{
		trans[i] = box2DColliders[i]->GetTransform();
		// 中心点を計算
		//auto pos = box2DColliders[i]->GetVertex();
		//posCenter[i] = Half<Vector2>(pos["LT"] + pos["RB"]);
	}

	// 当たり判定
	if (std::abs(posCenter[0].x - posCenter[1].x) <= Half(trans[0]->size.x + trans[1]->size.x) &&
		std::abs(posCenter[0].y - posCenter[1].y) <= Half(trans[0]->size.y + trans[1]->size.y))
	{
		return true;
	}
	return false;
}

//bool WE::_2D::CheckBox2Rays(const std::array<TwoRayCollider*, 2>& colliders)
//{
//	const TwoRayCollider* rayCollider = nullptr;
//	const BoxCollider* boxCollider = nullptr;
//	for (auto collider : colliders)
//	{
//		if (collider->GetShapeType() == ChangeVal(CollisionShapeType::Box))
//		{
//			boxCollider = dynamic_cast<BoxCollider*>(collider);
//		}
//		if (collider->GetShapeType() == ChangeVal(CollisionShapeType::TwoRay))
//		{
//			rayCollider = dynamic_cast<TwoRayCollider*>(collider);
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