#include "CollisionManager2D.h"
#include <algorithm>
#include <cassert>
using namespace WE;
using namespace _2D;

CollisionManager* CollisionManager::GetInstance()
{
	static CollisionManager instance;
	return &instance;
}

BaseColliderGroup* CollisionManager::AddGroup(CR<std::string> groupName, const std::optional<ColliderInfo>& info)
{
	if (!colliderGroups.contains(groupName))
	{
		uPtr<BaseColliderGroup> newGroup = std::make_unique<BaseColliderGroup>(groupName);
		if (info) { newGroup->SetColliderInfo(*info); }
		colliderGroups[groupName] = std::move(newGroup);
	}
	return colliderGroups[groupName].get();
}

BaseColliderGroup* CollisionManager::GetGroup(CR<std::string> groupName)
{
	if (!colliderGroups.contains(groupName)) { return nullptr; }
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
			BaseColliderGroup* groupA = itrA->second.get();
			BaseColliderGroup* groupB = itrB->second.get();
			if (!CheckFiltering(groupA, groupB)) { continue; }
			if (!Check2Groups(groupA, groupB)) { continue; }
		}
	}

	for (auto& group : colliderGroups)
	{
		group.second->CallCollisions();
	}
}

bool CollisionManager::Check2Groups(BaseColliderGroup* groupA, BaseColliderGroup* groupB)
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
				BaseCollisionPair pairA(colA, colB, hitInfo.get());
				BaseCollisionPair pairB(colB, colA, hitInfo.get());
				groupA->AddCollisionPair(pairA);
				groupB->AddCollisionPair(pairB);
				hitInfo->Reset();
			}
		}
	}

	return !groupA->GetCollisionPairs().empty();
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