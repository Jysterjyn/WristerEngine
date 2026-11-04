#include "CollisionManager2D.h"
#include "Collider2D.h"
#include <algorithm>
#include <cassert>
using namespace WE;
using namespace _2D;

CollisionManager* CollisionManager::GetInstance()
{
	static CollisionManager instance;
	return &instance;
}

bool CollisionManager::Check2Collisions(BaseSingleCollider* colliderA, BaseSingleCollider* colliderB)
{
	uint32_t aST = colliderA->GetShapeType();
	uint32_t bST = colliderB->GetShapeType();

	assert(aST != (uint32_t)CollisionShapeType::Unknown);
	assert(bST != (uint32_t)CollisionShapeType::Unknown);

	switch (aST)
	{
	case (uint32_t)CollisionShapeType::Circle:
	{
		CircleCollider* sphere = static_cast<CircleCollider*>(colliderA);

		switch (bST)
		{
		case (uint32_t)CollisionShapeType::Circle:

			return Check2Circles(sphere, static_cast<CircleCollider*>(colliderB));
		}
		break;
	}
	}

	return false;
}