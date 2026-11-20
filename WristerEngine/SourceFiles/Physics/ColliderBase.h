#pragma once
#include <cstdint>
#include <WristerEngineUtility.h>
#include <optional>
#include <map>

namespace WristerEngine
{
	class BaseCollider;

	// コライダー情報
	struct ColliderInfo
	{
	protected:
		uint32_t attribute = 0;
		uint32_t mask = UINT32_MAX;
		std::string name;
		BaseCollider* owner = nullptr;

		void SetColliderInfo(CR<ColliderInfo> info);
		friend class ColliderGroup;

	public:
		ColliderInfo() = default;
		ColliderInfo(uint32_t attribute, uint32_t mask, CR<std::string> name = "");

		virtual ~ColliderInfo() = default;

		// setter
		void SetAttribute(uint32_t attribute_) { attribute = attribute_; }
		void SetMask(uint32_t mask_) { mask = mask_; }
		void SetName(CR<std::string> name_) { name = name_; }
		void SetOwner(BaseCollider* owner_) { owner = owner_; }
		// getter
		uint32_t GetAttribute() const { return attribute; }
		uint32_t GetMask() const { return mask; }
		CR<ColliderInfo> GetColliderInfo() const { return *this; }
		std::string GetName() const { return name; }
		BaseCollider* GetOwner() { return owner; }
	};

	class QueryCallback;
	struct CollisionPair;

	// コライダーの基底クラス
	class BaseSingleCollider : public ColliderInfo, public ListObject
	{
	private:
		static uint32_t nextSerialNumber;

	protected:
		bool isDestroy = false;
		uint32_t serialNumber = 0;
		uint32_t shapeType = 0;
		QueryCallback* queryCallback = nullptr;

	public:
		BaseSingleCollider() { serialNumber = nextSerialNumber++; }
		virtual ~BaseSingleCollider() = default;
		virtual void Initialize() override {}
		virtual void Update() override {}
		void Destroy() { isDestroy = true; }
		bool Remove() const override { return isDestroy; }

		void ColliderInitialize(BaseCollider* owner_, CR<ColliderInfo> info, QueryCallback* callback);
		void OnQueryHit(CollisionPair* info) const;

		// getter
		bool IsDestroy() const { return isDestroy; }
		uint32_t GetSerialNumber() const { return serialNumber; }
		uint32_t GetShapeType() const { return shapeType; }
	};

	// 衝突情報
	struct HitInfo
	{
		float distance = 0.0f;

		void Reset() {}
		virtual ~HitInfo() = default;
	};

	// 衝突ペア
	struct CollisionPair
	{
		BaseSingleCollider* my = nullptr, * other = nullptr;
		HitInfo* hitInfo = nullptr;

		CollisionPair(BaseSingleCollider* my, BaseSingleCollider* other, HitInfo* hitInfo = nullptr)
			: my(my), other(other), hitInfo(hitInfo)
		{
		}

		void OnQueryHit() { return my->OnQueryHit(this); }
	};

	// クエリーで交差を検出した時の動作を規定するクラス
	class QueryCallback
	{
	public:
		virtual ~QueryCallback() = default;

		/// <summary>
		/// 交差時コールバック
		/// </summary>
		/// <param name="info">交差情報</param>
		virtual void OnQueryHit(CollisionPair* pair) = 0;
	};

	using CollisionPairList = std::list<CollisionPair>;

	class IColliderFactory
	{
	public:
		virtual ~IColliderFactory() = default;
		virtual uPtr<BaseSingleCollider> CreateCollider(uint32_t colliderShapeType) = 0;
	};

	// コライダーグループ
	class ColliderGroup : public ColliderInfo
	{
	private:
		uList colliders;
		IColliderFactory* factory = nullptr;

		// 当たったペアの記録
		CollisionPairList collisionPairs, enterPairs, exitPairs, collisionPairsPre;

		// コールバック関数呼び出し
		void CallCallbacks(CR<CollisionPairList> pairs, void (BaseCollider::* callback)());
		void OneCallPair(CollisionPairList& at, CR<CollisionPairList> pair1, CR<CollisionPairList> pair2);
		void CallEnter();
		void CallExit();

	public:
		ColliderGroup(CR<std::string> groupName) { SetName(groupName); }
		~ColliderGroup();

		void Initialize(IColliderFactory* factory, BaseCollider* owner, std::optional<ColliderInfo>info);
		void Update();

		/// <summary>
		/// コライダーを登録
		/// </summary>
		/// <param name="shapeType">コライダーの形状</param>
		/// <returns>登録されたコライダー</returns>
		BaseSingleCollider* AddCollider(uint32_t colliderShapeType, std::optional<ColliderInfo> info = std::nullopt, QueryCallback* callback = nullptr);

		void SetFactory(IColliderFactory* factory_) { factory = factory_; }
		void AddCollisionPair(const CollisionPair& pair) { collisionPairs.push_back(pair); }

		void CallCollisions();

		// getter
		const uList* GetColliders() const { return &colliders; }
		CollisionPairList& GetCollisionPairs() { return collisionPairs; }
		CollisionPairList& GetEnterPairs() { return enterPairs; }
		CollisionPairList& GetExitPairs() { return exitPairs; }
	};

	// コライダーの基底クラス
	class BaseCollider
	{
	private:
		static uint32_t nextSerialNumber;

	protected:
		uint32_t serialNumber = 0;
		ColliderGroup* group = nullptr;

	public:
		virtual ColliderGroup* Initialize(CR<ColliderInfo> info) = 0;

		// 衝突コールバック関数
		// 当たっている間
		virtual void OnCollision() {}
		// 当たった瞬間
		virtual void OnCollisionEnter() {}
		// 離れた瞬間
		virtual void OnCollisionExit() {}

		BaseCollider() { serialNumber = nextSerialNumber++; }
		virtual ~BaseCollider();

		void DeleteGroup() { group = nullptr; }
		// getter
		CollisionPairList& GetCollisionPairs() { return group->GetCollisionPairs(); }
		ColliderGroup* GetGroup() const { return group; }
		uint32_t GetSerialNumber() const { return serialNumber; }
	};

	// 当たり判定クラス
	class BaseCheckCollision : public CollisionPair
	{
	public:
		BaseCheckCollision(BaseSingleCollider* colliderA, BaseSingleCollider* colliderB) :
			CollisionPair(colliderA, colliderB)
		{
		}
		virtual bool Check2Collisions() = 0;
		CollisionPair GetRevercedPair() const { return CollisionPair(other, my, hitInfo); }
	};

	bool CheckCollisionPair(const CollisionPair& p1, const CollisionPair& p2);
	bool CheckFiltering(const ColliderInfo* infoA, const ColliderInfo* infoB);
}