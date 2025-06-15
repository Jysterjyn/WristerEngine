#pragma once
#include "Collider.h"
#include <list>
#include <optional>

namespace WristerEngine
{
	// コライダー管理
	class CollisionManager final
	{
	private:
		std::unordered_map<std::string, std::unique_ptr<ColliderGroup>> colliderGroups;
		bool isPrint = false;
		std::optional<Vector3> inter = std::nullopt;

		CollisionManager() = default;
		~CollisionManager() = default;
		CollisionManager(const CollisionManager&) = delete;
		CollisionManager& operator=(const CollisionManager&) = delete;

		bool CheckCollisionFiltering(const CollisionInfo* infoA, const CollisionInfo* infoB);

		bool Check2Collisions(BaseCollider* colliderA, BaseCollider* colliderB);

		// 個別当たり判定
		//bool Check2DCollision2Boxes(const std::array<_2D::Base2DCollider*, 2>& colliders);
		//bool Check2DCollisionBox2Rays(const std::array<_2D::Base2DCollider*, 2>& colliders);

		bool CheckCollision2Groups(ColliderGroup* colliderGroupA, ColliderGroup* colliderGroupB);
		bool CheckCollision2Spheres(const SphereCollider* colliderA, const SphereCollider* colliderB);
		bool CheckCollisionSpherePlane(const SphereCollider* colliderA, const PlaneCollider* colliderB);
		bool CheckCollisionSphereTriangle(const SphereCollider* colliderA, const TriangleCollider* colliderB);
		//bool CheckCollisionRaySphere(RayCollider* colliderA, SphereCollider* colliderB, float* distance = nullptr, Vector3* inter = nullptr);
		//bool CheckCollision2Boxes(BoxCollider* colliderA, BoxCollider* colliderB);
		//bool CheckCollision2IncludeObjects(IncludeCollider* colliderA, IncludeCollider* colliderB);
		//bool CheckCollisionRayPlane(RayCollider* colliderA, PlaneCollider* colliderB, float* distance = nullptr);
		//bool CheckCollisionRayBox(RayCollider* colliderA, BoxCollider* colliderB);
		//bool CheckCollisionRayPolygon(RayCollider* colliderA, PolygonCollider* colliderB, float* distance = nullptr);

	public:
		static CollisionManager* GetInstance();

		ColliderGroup* AddGroup(const std::string& groupName);

		// 全当たり判定
		void CheckCollisions();

		bool IsPrint() const { return isPrint; }
	};
}