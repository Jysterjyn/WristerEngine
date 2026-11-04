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
	struct QueryHit : public BaseQueryHit
	{
		// 衝突点
		std::optional<Vector2> inter = std::nullopt;
		// 排斥ベクトル
		std::optional<Vector2> reject = std::nullopt;
	};

	// コライダー管理
	class CollisionManager final
	{
	private:
		uPtr<HitInfo> hitInfo;
		std::unordered_map<std::string, uPtr<BaseColliderGroup>> colliderGroups;
		
		CollisionManager() = default;
		~CollisionManager() = default;
		CollisionManager(const CollisionManager&) = delete;
		CollisionManager& operator=(const CollisionManager&) = delete;

		bool Check2Collisions(BaseSingleCollider* colliderA, BaseSingleCollider* colliderB);

		// 個別当たり判定
		bool Check2Groups(BaseColliderGroup* groupA, BaseColliderGroup* groupB);

	public:
		static CollisionManager* GetInstance();
		BaseColliderGroup* AddGroup(CR<std::string> groupName, const std::optional<ColliderInfo>& info = std::nullopt);
		BaseColliderGroup* GetGroup(CR<std::string> groupName);

		// 全当たり判定
		void CheckCollisions();
	};
}