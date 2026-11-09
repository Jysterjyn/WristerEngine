#pragma once
#include "ColliderBase.h"
#include <Vector.h>
#include <list>
#include <optional>
#include <unordered_map>

namespace WristerEngine
{
	/// <summary>
	/// クエリーによる情報を得る為の構造体
	/// </summary>
	struct BaseQueryHit
	{
		// 衝突相手のコライダー
		BaseSingleCollider* collider = nullptr;
	};

	/// <summary>
	/// クエリーで交差を検出した時の動作を規定するクラス
	/// </summary>
	class QueryCallback
	{
	public:
		virtual ~QueryCallback() = default;

		/// <summary>
		/// 交差時コールバック
		/// </summary>
		/// <param name="info">交差情報</param>
		/// <returns>クエリーを続けるならtrue、打ち切るならfalseを返す</returns>
		virtual bool OnQueryHit(CR<BaseQueryHit> info) = 0;
	};

	namespace _2D
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
	}

	namespace _3D
	{
		/// <summary>
		/// クエリーによる情報を得る為の構造体
		/// </summary>
		struct QueryHit : public BaseQueryHit
		{
			// 衝突点
			std::optional<Vector3> inter = std::nullopt;
			// 排斥ベクトル
			std::optional<Vector3> reject = std::nullopt;
		};
	}

	class BaseColliderManager
	{
	protected:
		std::unordered_map<std::string, uPtr<ColliderGroup>> colliderGroups;
		uPtr<HitInfo> hitInfo;
		uPtr<IColliderFactory> factory;

	public:
		virtual ~BaseColliderManager() = default;

		// コライダーグループの追加と取得
		ColliderGroup* AddGroup(BaseCollider* owner, CR<ColliderInfo> info);
		// コライダーグループの取得
		ColliderGroup* GetGroup(CR<std::string> groupName);
		// 2つのコライダーの当たり判定
		virtual bool Check2Collisions(BaseSingleCollider* colliderA, BaseSingleCollider* colliderB) = 0;
		// グループ単位での当たり判定
		bool Check2Groups(ColliderGroup* groupA, ColliderGroup* groupB);
		// 全当たり判定
		void CheckCollisions();
	};

	namespace _2D
	{
		class ColliderManager2D : public BaseColliderManager
		{
		public:
			ColliderManager2D();
			bool Check2Collisions(BaseSingleCollider* colliderA, BaseSingleCollider* colliderB) override;
		};
	}

	namespace _3D
	{
		class ColliderManager3D : public BaseColliderManager
		{
		public:
			ColliderManager3D();
			bool Check2Collisions(BaseSingleCollider* colliderA, BaseSingleCollider* colliderB) override;
		};
	}

	enum class ColliderDimension
	{
		_2D, _3D
	};

	// コライダー管理
	class ColliderManager final
	{
	private:
		_2D::ColliderManager2D colliderManager2D;
		_3D::ColliderManager3D colliderManager3D;

		ColliderManager() = default;
		~ColliderManager() = default;
		ColliderManager(const ColliderManager&) = delete;
		ColliderManager& operator=(const ColliderManager&) = delete;

	public:
		static ColliderManager* GetInstance();

		BaseColliderManager* GetColliderManager(ColliderDimension dimension);

		// 全当たり判定
		void CheckCollisions();
	};
}