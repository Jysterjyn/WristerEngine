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
		std::optional<float> distance = std::nullopt;

		CollisionManager() = default;
		~CollisionManager() = default;
		CollisionManager(const CollisionManager&) = delete;
		CollisionManager& operator=(const CollisionManager&) = delete;

		bool CheckFiltering(const CollisionInfo* infoA, const CollisionInfo* infoB);

		bool Check2Collisions(BaseCollider* colliderA, BaseCollider* colliderB);

		// 個別当たり判定
		//bool Check2DCollision2Boxes(const std::array<_2D::Base2DCollider*, 2>& colliders);
		//bool Check2DCollisionBox2Rays(const std::array<_2D::Base2DCollider*, 2>& colliders);

		bool Check2Groups(ColliderGroup* groupA, ColliderGroup* groupB);
		bool Check2Spheres(const SphereCollider* sphereA, const SphereCollider* sphereB);
		bool CheckSpherePlane(const SphereCollider* sphere, const PlaneCollider* plane);
		bool CheckSphereTriangle(const SphereCollider* sphere, const TriangleCollider* triangle);
		bool Check2Boxes(const BoxCollider* boxA, const BoxCollider* boxB);
		bool Check2IncludeBoxes(const IncludeCollider* iBoxA, const IncludeCollider* iBoxB);
		bool CheckRayPlane(const RayCollider* ray, const PlaneCollider* plane);
		bool CheckRayTriangle(const RayCollider* ray, const TriangleCollider* triangle);
		//bool CheckCollisionRaySphere(RayCollider* colliderA, SphereCollider* colliderB, float* distance = nullptr, Vector3* inter = nullptr);
		//bool CheckCollisionRayBox(RayCollider* colliderA, BoxCollider* colliderB);

	public:
		static CollisionManager* GetInstance();

		ColliderGroup* AddGroup(const std::string& groupName);

		// 全当たり判定
		void CheckCollisions();

		bool IsPrint() const { return isPrint; }
	};
}