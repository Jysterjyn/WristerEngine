#pragma once
#include "Collider.h"
#include <list>

namespace WristerEngine
{
	// コライダー管理
	class CollisionManager final
	{
	private:
		std::list<SphereCollider*> sphereColliders;
		//std::list<BoxCollider*> boxColliders;
		//std::list<IncludeCollider*> includeColliders;
		//std::list<PlaneCollider*> planeColliders;
		//std::list<PolygonCollider*> polygonColliders;
		//std::list<RayCollider*> rayColliders;
		//std::list<_2D::ColliderGroup*> _2DColliders;
		std::unordered_map<std::string, std::unique_ptr<ColliderGroup>> colliderGroups;
		bool isPrint;

		CollisionManager() = default;
		~CollisionManager() = default;
		CollisionManager(const CollisionManager&) = delete;
		CollisionManager& operator=(const CollisionManager&) = delete;

	public:
		static CollisionManager* GetInstance();

		bool CheckCollisionFiltering(const CollisionInfo* infoA, const CollisionInfo* infoB);
		// 個別当たり判定
		bool CheckCollision2Spheres(SphereCollider* colliderA, SphereCollider* colliderB);
		bool CheckCollision2Groups(ColliderGroup* colliderGroupA, ColliderGroup* colliderGroupB);
		//bool CheckCollisionFiltering(_2D::ColliderGroup* colliderA, _2D::ColliderGroup* colliderB);
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

		// コライダー登録関数
		void PushCollider(SphereCollider* collider) { sphereColliders.push_back(collider); }
		void PushColliderGroup(const std::string& groupName, std::unique_ptr<ColliderGroup> collider) { colliderGroups[groupName] = std::move(collider); }
		// void PushCollider(BoxCollider* collider) { boxColliders.push_back(collider); }
		// void PushCollider(IncludeCollider* collider) { includeColliders.push_back(collider); }
		// void PushCollider(PlaneCollider* collider) { planeColliders.push_back(collider); }
		// void PushCollider(PolygonCollider* collider) { polygonColliders.push_back(collider); }
		// void PushCollider(RayCollider* collider) { rayColliders.push_back(collider); }
		// void PushCollider(_2D::ColliderGroup* collider) { _2DColliders.push_back(collider); }

		// コライダー削除関数
		void PopCollider(SphereCollider* collider) { sphereColliders.remove(collider); }
		// void PopCollider(BoxCollider* collider) { boxColliders.remove(collider); }
		// void PopCollider(IncludeCollider* collider) { includeColliders.remove(collider); }
		// void PopCollider(PlaneCollider* collider) { planeColliders.remove(collider); }
		// void PopCollider(PolygonCollider* collider) { polygonColliders.remove(collider); }
		// void PopCollider(RayCollider* collider) { rayColliders.remove(collider); }
		// void PopCollider(_2D::ColliderGroup* collider) { _2DColliders.remove(collider); }
		// void PopAll2DCollider() { _2DColliders.clear(); }
		// 種類別当たり判定
		void CheckSphereCollisions();
		void CheckGroupCollisions();
		// void CheckBoxCollisions();
		// void CheckIncludeCollisions();
		// void CheckSpherePlaneCollisions();
		// void CheckSpherePolygonCollisions();
		// void CheckRayPlaneCollisions();
		// void CheckRayPolygonCollisions();
		// void CheckRaySphereCollisions();
		// void CheckRayBoxCollisions();
		// void CheckRayCastCollision(RayCollider* collider);
		// void Check2DCollisions();
		// bool CheckCollision2ColliderGroups(_2D::ColliderGroup* groupA, _2D::ColliderGroup* groupB);
		// 全当たり判定
		void CheckAllCollisions();

		bool IsPrint() const { return isPrint; }
	};
}