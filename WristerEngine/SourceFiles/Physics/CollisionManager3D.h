#pragma once
#include "Collider3D.h"
#include <list>
#include <optional>

namespace WristerEngine::_3D
{
	struct RaycastHit
	{
		//衝突相手のコライダー
		BaseCollider* collider = nullptr;
		//衝突点
		Vector3 inter;
		//衝突点までの距離
		float distance = 0.0f;
	};

	/// <summary>
	/// クエリーによる情報を得る為の構造体
	/// </summary>
	struct QueryHit
	{
		// 衝突相手のコライダー
		BaseCollider* collider = nullptr;
		// 衝突点
		Vector3 inter;
		// 排斥ベクトル
		Vector3 reject;
	};

	/// <summary>
	/// クエリーで交差を検出した時の動作を規定するクラス
	/// </summary>
	class QueryCallback
	{
	public:
		QueryCallback() = default;
		virtual ~QueryCallback() = default;

		/// <summary>
		/// 交差時コールバック
		/// </summary>
		/// <param name="info">交差情報</param>
		/// <returns>クエリーを続けるならtrue、打ち切るならfalseを返す</returns>
		virtual bool OnQueryHit(const QueryHit& info) = 0;
	};

	// コライダー管理
	class CollisionManager final : private HitInfo
	{
	private:
		std::unordered_map<std::string, std::unique_ptr<ColliderGroup>> colliderGroups;

		CollisionManager() = default;
		~CollisionManager() = default;
		CollisionManager(const CollisionManager&) = delete;
		CollisionManager& operator=(const CollisionManager&) = delete;

		bool Check2Collisions(BaseCollider* colliderA, BaseCollider* colliderB);

		// 個別当たり判定
		bool Check2Groups(ColliderGroup* groupA, ColliderGroup* groupB);
		bool Check2Spheres(const SphereCollider* sphereA, const SphereCollider* sphereB);
		bool CheckSpherePlane(const SphereCollider* sphere, const PlaneCollider* plane);
		bool CheckSphereTriangle(const SphereCollider* sphere, const TriangleCollider* triangle);
		bool CheckSphereMesh(const SphereCollider* sphere, const MeshCollider* mesh);
		bool Check2Boxes(const BoxCollider* boxA, const BoxCollider* boxB);
		bool Check2IncludeBoxes(const IncludeCollider* iBoxA, const IncludeCollider* iBoxB);
		bool CheckRayPlane(const RayCollider* ray, const PlaneCollider* plane);
		bool CheckRayTriangle(const RayCollider* ray, const TriangleCollider* triangle);
		bool CheckRaySphere(const RayCollider* ray, const SphereCollider* sphere);
		bool CheckRayMesh(const RayCollider* ray, const MeshCollider* mesh);
		//bool CheckCollisionRayBox(RayCollider* colliderA, BoxCollider* colliderB);

	public:
		static CollisionManager* GetInstance();
		ColliderGroup* AddGroup(const std::string& groupName, const std::optional<BaseInfo>& info = std::nullopt);

		/// <summary>
		/// レイキャスト
		/// </summary>
		/// <param name="ray">レイコライダー</param>
		/// <param name="attribute">対象の衝突属性</param>
		/// <param name="hitInfo">衝突情報</param>
		/// <param name="maxDistance">最大距離</param>
		/// <returns>レイが任意のコライダーと交わる場合はtrue、それ以外はfalse</returns>
		bool Raycast(const RayCollider* ray, uint32_t attribute, RaycastHit* hitInfo = nullptr,
			const float maxDistance = D3D12_FLOAT32_MAX);

		/// <summary>
		/// 球による衝突全検索
		/// </summary>
		/// <param name="sphere">球コライダー</param>
		/// <param name="callback">衝突時コールバック</param>
		/// <param name="attribute">対象の衝突属性</param>
		void QuerySphere(const SphereCollider* sphere, QueryCallback* callback, uint32_t attribute = UINT32_MAX);

		// 全当たり判定
		void CheckCollisions();
	};
}