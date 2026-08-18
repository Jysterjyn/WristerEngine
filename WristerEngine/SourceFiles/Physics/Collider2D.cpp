#include "Collider2D.h"
#include <cassert>
using namespace WE;
using namespace _2D;

uint32_t SingleCollider2D::nextSerialNumber = 0;
uint32_t Collider::nextSerialNumber = 0;

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
