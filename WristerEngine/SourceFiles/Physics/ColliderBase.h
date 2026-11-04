#pragma once
#include <cstdint>
#include <WristerEngineUtility.h>
#include <optional>
#include <map>

namespace WristerEngine
{
	struct ColliderInfo
	{
	protected:
		uint32_t attribute = 0;
		uint32_t mask = UINT32_MAX;
		OneChange<std::string> name;

	public:
		ColliderInfo() = default;
		ColliderInfo(uint32_t attribute_, uint32_t mask_);
		ColliderInfo(const ColliderInfo& info);

		virtual ~ColliderInfo() = default;

		// setter
		void SetAttribute(uint32_t attribute_) { attribute = attribute_; }
		void SetMask(uint32_t mask_) { mask = mask_; }
		void SetColliderInfo(const ColliderInfo& info);
		void SetName(CR<std::string> name_) { name = name_; }
		// getter
		uint32_t GetAttribute() const { return attribute; }
		uint32_t GetMask() const { return mask; }
		ColliderInfo GetColliderInfo() const;
		std::string GetName() const { return name.Get(); }
	};

	class BaseCollider;

	class BaseSingleCollider : public ColliderInfo, public ListObject
	{
	protected:
		bool isDestroy = false;
		BaseCollider* owner = nullptr;
		uint32_t serialNumber = 0;
		uint32_t shapeType = 0;

	public:
		virtual ~BaseSingleCollider() = default;
		virtual void Initialize() override {}
		virtual void Update() override {}
		void Destroy() { isDestroy = true; }
		bool Remove() const override { return isDestroy; }

		void ColliderInitialize(BaseCollider* owner_, const ColliderInfo& info)
		{
			owner = owner_; SetColliderInfo(info);
		}

		// getter
		bool IsDestroy() const { return isDestroy; }
		BaseCollider* GetOwner() { return owner; }
		uint32_t GetSerialNumber() const { return serialNumber; }
		uint32_t GetShapeType() const { return shapeType; }
	};

	struct HitInfo
	{
		std::optional<float> distance = std::nullopt;

		void Reset() {}
		virtual ~HitInfo() = default;
	};

	struct BaseCollisionPair
	{
		BaseSingleCollider* my = nullptr, * other = nullptr;
		HitInfo* hitInfo = nullptr;

		BaseCollisionPair(BaseSingleCollider* my_, BaseSingleCollider* other_, HitInfo* hitInfo_)
		{
			my = my_; other = other_; hitInfo = hitInfo_;
		}
	};

	using CollisionPairList = std::list<BaseCollisionPair>;

	class BaseCollisionManager;

	class BaseColliderGroup : public ColliderInfo
	{
	protected:
		uList colliders;
		BaseCollider* owner = nullptr;
		BaseCollisionManager* manager = nullptr;

		// 当たったペアの記録
		CollisionPairList collisionPairs, enterPairs, exitPairs, collisionPairsPre;

	private:
		// コールバック関数呼び出し
		void CallCallbacks(CR<CollisionPairList> pairs, void (BaseCollider::* callback)());
		void OneCallPair(CollisionPairList& at, CR<CollisionPairList> pair1, CR<CollisionPairList> pair2);
		void CallEnter();
		void CallExit();

	public:
		BaseColliderGroup(CR<std::string> groupName) { SetName(groupName); }
		~BaseColliderGroup();

		void Update();

		/// <summary>
		/// コライダーを登録
		/// </summary>
		/// <param name="shapeType">コライダーの形状</param>
		/// <returns>登録されたコライダー</returns>
		BaseSingleCollider* AddCollider(uPtr<BaseSingleCollider> newCollider);

		void SetOwner(BaseCollider* owner_) { owner = owner_; }
		void AddCollisionPair(const BaseCollisionPair& pair) { collisionPairs.push_back(pair); }

		void CallCollisions();

		// getter
		const uList* GetColliders() const { return &colliders; }
		BaseCollider* GetOwner() const { return owner; }
		CR<CollisionPairList> GetCollisionPairs() const { return collisionPairs; }
		CR<CollisionPairList> GetEnterPairs() const { return enterPairs; }
		CR<CollisionPairList> GetExitPairs() const { return exitPairs; }
	};

	class IColliderFactory
	{
	public:
		virtual ~IColliderFactory() = default;
		virtual BaseSingleCollider* CreateCollider(CR<std::string> groupName, CR<ColliderInfo> info) = 0;
	};

	class BaseCollider
	{
	protected:
		uint32_t serialNumber = 0;

		BaseColliderGroup* group = nullptr;

		template<class T>
		bool TypeCompare(CR<std::string> type) const
		{
			const std::string TYPE_NAME = typeid(type).name();
			return TYPE_NAME.find(type) != std::string::npos;
		}

	public:
		/// <summary>
		/// コライダーを登録
		/// </summary>
		/// <param name="shapeType">コライダーの形状</param>
		/// <returns>登録されたコライダー</returns>
		//template<class T>
		//T* AddCollider(const std::optional<ColliderInfo>& info = std::nullopt)
		//{
		//	// コライダーの型がColliderであるかを確認
		//	const std::string TYPE_NAME = typeid(T).name();
		//	auto TypeCompare = [&TYPE_NAME](CR<std::string> type) { return TYPE_NAME.find(type) != std::string::npos; };
		//	if (!TypeCompare("Collider")) { return nullptr; }

		//	// コライダーグループが登録されていない場合は登録する
		//	uPtr<BaseSingleCollider> newCollider;
		//	ColliderInfo colliderInfo(group->GetColliderInfo());
		//	std::string colliderName = info ? info->GetName() : group->GetName();
		//	colliderInfo.SetName(colliderName);

		//	// コライダー情報が指定されている場合は上書きする
		//	newCollider = std::make_unique<T>();
		//	newCollider->Initialize(this, colliderInfo);
		//	return static_cast<T*>(group->AddCollider(std::move(newCollider)));
		//}

		void Initialize(CR<std::string> groupName, const std::optional<ColliderInfo>& info = std::nullopt);

		// 衝突コールバック関数
		// 当たっている間
		virtual void OnCollision() {}
		// 当たった瞬間
		virtual void OnCollisionEnter() {}
		// 離れた瞬間
		virtual void OnCollisionExit() {}

		virtual ~BaseCollider();

		void DeleteGroup() { group = nullptr; }
		// getter
		CR<CollisionPairList> GetCollisionPairs() const { return group->GetCollisionPairs(); }
		BaseColliderGroup* GetGroup() const { return group; }
		uint32_t GetSerialNumber() const { return serialNumber; }
	};

	class BaseCollisionManager
	{
		uPtr<HitInfo> hitInfo;
		std::unordered_map<std::string, uPtr<BaseColliderGroup>> colliderGroups;

	public:
		virtual ~BaseCollisionManager() = default;

		BaseColliderGroup* AddGroup(CR<std::string> groupName, const std::optional<ColliderInfo>& info = std::nullopt);
		BaseColliderGroup* GetGroup(CR<std::string> groupName);

		virtual bool Check2Collisions(BaseSingleCollider* colliderA, BaseSingleCollider* colliderB) = 0;

		// グループ単位での当たり判定
		bool Check2Groups(BaseColliderGroup* groupA, BaseColliderGroup* groupB);

		// 全当たり判定
		void CheckCollisions();
	};

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

	bool CheckCollisionPair(const BaseCollisionPair& p1, const BaseCollisionPair& p2);
	bool CheckFiltering(const ColliderInfo* infoA, const ColliderInfo* infoB);
}