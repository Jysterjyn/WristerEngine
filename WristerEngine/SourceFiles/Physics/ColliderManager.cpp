#include "ColliderManager.h"
#include "Collider2D.h"
#include <algorithm>
#include <cassert>
using namespace WE;

ColliderGroup* BaseColliderManager::AddGroup(CR<std::string> groupName, BaseCollider* owner, const std::optional<ColliderInfo>& info)
{
	if (!colliderGroups.contains(groupName))
	{
		uPtr<ColliderGroup> newGroup = std::make_unique<ColliderGroup>(groupName);
		newGroup->Initialize(factory.get(), owner, info);
		colliderGroups[groupName] = std::move(newGroup);
	}
	return GetGroup(groupName);
}

ColliderGroup* BaseColliderManager::GetGroup(CR<std::string> groupName)
{
	if (!colliderGroups.contains(groupName)) { return nullptr; }
	return colliderGroups[groupName].get();
}

void BaseColliderManager::CheckCollisions()
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
		group.second->CallCollisions();
	}
}

bool BaseColliderManager::Check2Groups(ColliderGroup* groupA, ColliderGroup* groupB)
{
	for (const auto& colliderA : *groupA->GetColliders()) {
		for (const auto& colliderB : *groupB->GetColliders())
		{
			BaseSingleCollider* colA = static_cast<BaseSingleCollider*>(colliderA.get());
			BaseSingleCollider* colB = static_cast<BaseSingleCollider*>(colliderB.get());
			if (!CheckFiltering(colA, colB)) { continue; }

			std::list<BaseSingleCollider*> colliderPair({ colA,colB });
			colliderPair.sort([](BaseSingleCollider* c1, BaseSingleCollider* c2)
				{
					return c1->GetShapeType() < c2->GetShapeType();
				});

			if (Check2Collisions(colliderPair.front(), colliderPair.back()))
			{
				CollisionPair pairA(colA, colB, hitInfo.get());
				CollisionPair pairB(colB, colA, hitInfo.get());
				groupA->AddCollisionPair(pairA);
				groupB->AddCollisionPair(pairB);
				hitInfo->Reset();
			}
		}
	}

	return !groupA->GetCollisionPairs().empty();
}

_2D::ColliderManager2D::ColliderManager2D()
{
	factory = std::make_unique<ColliderFactory>();
}

bool _2D::ColliderManager2D::Check2Collisions(BaseSingleCollider* colliderA, BaseSingleCollider* colliderB)
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

_3D::ColliderManager3D::ColliderManager3D()
{
	//factory = std::make_unique<ColliderFactory>();
}

bool _3D::ColliderManager3D::Check2Collisions(BaseSingleCollider* colliderA, BaseSingleCollider* colliderB)
{
	uint32_t aST = colliderA->GetShapeType();
	uint32_t bST = colliderB->GetShapeType();

	//assert(aST != (uint32_t)CollisionShapeType::Unknown);
	//assert(bST != (uint32_t)CollisionShapeType::Unknown);

	switch (aST)
	{
	case 0:
	{
		//CircleCollider* sphere = static_cast<CircleCollider*>(colliderA);

		switch (bST)
		{
		case 0:

		//	return Check2Circles(sphere, static_cast<CircleCollider*>(colliderB));
			break;
		}
		break;
	}
	}

	return false;
}

ColliderManager* ColliderManager::GetInstance()
{
	static ColliderManager instance;
	return &instance;
}

BaseColliderManager* ColliderManager::GetColliderManager(ColliderDimension dimension)
{
	switch (dimension)
	{
	case ColliderDimension::_2D: return &colliderManager2D;
	case ColliderDimension::_3D: return &colliderManager3D;
	}
	return nullptr;
}

void ColliderManager::CheckCollisions()
{
	colliderManager2D.CheckCollisions();
	colliderManager3D.CheckCollisions();
}
