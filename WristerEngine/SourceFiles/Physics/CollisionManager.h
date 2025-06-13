#pragma once
#include "Collider.h"
#include <list>

namespace WristerEngine
{
	// コライダー管理
	class CollisionManager final
	{
	private:
		std::unordered_map<std::string, std::unique_ptr<ColliderGroup>> colliderGroups;
		bool isPrint = false;

		CollisionManager() = default;
		~CollisionManager() = default;
		CollisionManager(const CollisionManager&) = delete;
		CollisionManager& operator=(const CollisionManager&) = delete;

		// 個別当たり判定
		bool CheckCollision2Groups(ColliderGroup* colliderGroupA, ColliderGroup* colliderGroupB);
		bool CheckCollision2Spheres(SphereCollider* colliderA, SphereCollider* colliderB);
		//bool Check2DCollision2Boxes(const std::array<_2D::Base2DCollider*, 2>& colliders);
		//bool Check2DCollisionBox2Rays(const std::array<_2D::Base2DCollider*, 2>& colliders);
		//bool CheckCollision2Boxes(BoxCollider* colliderA, BoxCollider* colliderB);
		//bool CheckCollision2IncludeObjects(IncludeCollider* colliderA, IncludeCollider* colliderB);
		//bool CheckCollisionSpherePlane(SphereCollider* colliderA, PlaneCollider* colliderB, Vector3* inter = nullptr);
		//bool CheckCollisionSpherePolygon(SphereCollider* colliderA, PolygonCollider* colliderB, Vector3* inter = nullptr);
		//bool CheckCollisionRayPlane(RayCollider* colliderA, PlaneCollider* colliderB, float* distance = nullptr);
		//bool CheckCollisionRayPolygon(RayCollider* colliderA, PolygonCollider* colliderB, float* distance = nullptr);
		//bool CheckCollisionRaySphere(RayCollider* colliderA, SphereCollider* colliderB, float* distance = nullptr, Vector3* inter = nullptr);
		//bool CheckCollisionRayBox(RayCollider* colliderA, BoxCollider* colliderB);

	public:
		static CollisionManager* GetInstance();

		ColliderGroup* GetGroup(const std::string& groupName);

		bool CheckCollisionFiltering(const CollisionInfo* infoA, const CollisionInfo* infoB);

		// 全当たり判定
		void CheckCollisions();

		bool IsPrint() const { return isPrint; }
	};
}