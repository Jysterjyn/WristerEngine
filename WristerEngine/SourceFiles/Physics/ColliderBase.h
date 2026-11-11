#pragma once
#include <cstdint>
#include <WristerEngineUtility.h>
#include <optional>
#include <map>

namespace WristerEngine
{
	class BaseCollider;

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
		CR<ColliderInfo> GetColliderInfo() const;
		std::string GetName() const { return name; }
		BaseCollider* GetOwner() { return owner; }
	};

	class BaseSingleCollider : public ColliderInfo, public ListObject
	{
	private:
		static uint32_t nextSerialNumber;

	protected:
		bool isDestroy = false;
		uint32_t serialNumber = 0;
		uint32_t shapeType = 0;

	public:
		BaseSingleCollider() { serialNumber = nextSerialNumber++; }
		virtual ~BaseSingleCollider() = default;
		virtual void Initialize() override {}
		virtual void Update() override {}
		void Destroy() { isDestroy = true; }
		bool Remove() const override { return isDestroy; }

		void ColliderInitialize(BaseCollider* owner_, CR<ColliderInfo> info)
		{
			owner = owner_; SetColliderInfo(info);
		}

		// getter
		bool IsDestroy() const { return isDestroy; }
		uint32_t GetSerialNumber() const { return serialNumber; }
		uint32_t GetShapeType() const { return shapeType; }
	};

	struct HitInfo
	{
		std::optional<float> distance = std::nullopt;

		void Reset() {}
		virtual ~HitInfo() = default;
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
		virtual bool OnQueryHit(CR<HitInfo> info) = 0;
	};

	struct CollisionPair
	{
		BaseSingleCollider* my = nullptr, * other = nullptr;
		HitInfo* hitInfo = nullptr;

		CollisionPair(BaseSingleCollider* my, BaseSingleCollider* other, HitInfo* hitInfo)
			: my(my), other(other), hitInfo(hitInfo)
		{
		}
	};

	using CollisionPairList = std::list<CollisionPair>;

	class IColliderFactory
	{
	public:
		virtual ~IColliderFactory() = default;
		virtual uPtr<BaseSingleCollider> CreateCollider(uint32_t colliderShapeType) = 0;
	};

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
		BaseSingleCollider* AddCollider(uint32_t colliderShapeType, std::optional<ColliderInfo> info = std::nullopt);

		void SetFactory(IColliderFactory* factory_) { factory = factory_; }
		void AddCollisionPair(const CollisionPair& pair) { collisionPairs.push_back(pair); }

		void CallCollisions();

		// getter
		const uList* GetColliders() const { return &colliders; }
		CR<CollisionPairList> GetCollisionPairs() const { return collisionPairs; }
		CR<CollisionPairList> GetEnterPairs() const { return enterPairs; }
		CR<CollisionPairList> GetExitPairs() const { return exitPairs; }
	};

	class BaseCollider
	{
	private:
		static uint32_t nextSerialNumber;

	protected:
		uint32_t serialNumber = 0;

		ColliderGroup* group = nullptr;

		template<class T>
		bool TypeCompare(CR<std::string> type) const
		{
			const std::string TYPE_NAME = typeid(type).name();
			return TYPE_NAME.find(type) != std::string::npos;
		}

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
		CR<CollisionPairList> GetCollisionPairs() const { return group->GetCollisionPairs(); }
		ColliderGroup* GetGroup() const { return group; }
		uint32_t GetSerialNumber() const { return serialNumber; }
	};

	bool CheckCollisionPair(const CollisionPair& p1, const CollisionPair& p2);
	bool CheckFiltering(const ColliderInfo* infoA, const ColliderInfo* infoB);
}