#pragma once
#include "Collider2D.h"
#include <list>
#include <optional>
#include <unordered_map>

namespace WristerEngine::_2D
{
	/// <summary>
	/// クエリーによる情報を得る為の構造体
	/// </summary>
	struct QueryHit
	{
		// 衝突相手のコライダー
		BaseCollider2D* collider = nullptr;
		// 衝突点
		Vector2 inter;
		// 排斥ベクトル
		Vector2 reject;
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
	class CollisionManager final : private HitInfo<Vector2>
	{
	private:
		std::unordered_map<std::string, std::unique_ptr<ColliderGroup>> colliderGroups;
		
		CollisionManager() = default;
		~CollisionManager() = default;
		CollisionManager(const CollisionManager&) = delete;
		CollisionManager& operator=(const CollisionManager&) = delete;

		bool Check2Collisions(BaseCollider2D* colliderA, BaseCollider2D* colliderB);

		// 個別当たり判定
		bool Check2Groups(ColliderGroup* groupA, ColliderGroup* groupB);
		//bool Check2DCollision2Boxes(const std::array<_2D::Base2DCollider*, 2>& colliders);
		//bool Check2DCollisionBox2Rays(const std::array<_2D::Base2DCollider*, 2>& colliders);

	public:
		static CollisionManager* GetInstance();
		ColliderGroup* AddGroup(const std::string& groupName, const std::optional<ColliderInfo>& info = std::nullopt);

		// 全当たり判定
		void CheckCollisions();
	};
}