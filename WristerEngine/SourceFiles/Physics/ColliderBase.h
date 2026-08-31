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
		void SetName(const std::string& name_) { name = name_; }
		// getter
		uint32_t GetAttribute() const { return attribute; }
		uint32_t GetMask() const { return mask; }
		ColliderInfo GetColliderInfo() const;
		std::string GetName() const { return name.Get(); }
	};

	class BaseCollider;

	class BaseSingleCollider : public ColliderInfo
	{
	protected:
		bool isDestroy = false;
		BaseCollider* owner = nullptr;
		uint32_t serialNumber = 0;
		uint32_t shapeType = 0;

	public:
		virtual ~BaseSingleCollider() = default;
		virtual void Update() {}
		void Destroy() { isDestroy = true; }

		void Initialize(BaseCollider* owner_, const ColliderInfo& info)
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

	class BaseColliderGroup : public ColliderInfo
	{
	protected:
		std::list<uPtr<BaseSingleCollider>> colliders;
		std::list<BaseCollider*> owners;

		// 当たったペアの記録
		CollisionPairList collisionPairs, enterPairs, exitPairs, collisionPairsPre;

	private:
		// コールバック関数呼び出し
		void CallCallbacks(CR<CollisionPairList> pairs, void (BaseCollider::* callback)());
		void OneCallPair(CollisionPairList& at, CR<CollisionPairList> pair1, CR<CollisionPairList> pair2);
		void CallEnter();
		void CallExit();

	public:
		BaseColliderGroup(const std::string& groupName) { SetName(groupName); }
		~BaseColliderGroup();

		void Update();

		/// <summary>
		/// コライダーを登録
		/// </summary>
		/// <param name="shapeType">コライダーの形状</param>
		/// <returns>登録されたコライダー</returns>
		BaseSingleCollider* AddCollider(std::unique_ptr<BaseSingleCollider> newCollider);

		void AddOwner(BaseCollider* owner) { owners.push_back(owner); }
		void AddCollisionPair(const BaseCollisionPair& pair) { collisionPairs.push_back(pair); }

		void CallCollisions();

		// getter
		const std::list<uPtr<BaseSingleCollider>>* GetColliders() const { return &colliders; }
		CR<CollisionPairList> GetCollisionPairs() const { return collisionPairs; }
		CR<CollisionPairList> GetEnterPairs() const { return enterPairs; }
		CR<CollisionPairList> GetExitPairs() const { return exitPairs; }
	};

	class BaseCollider
	{
	protected:
		uint32_t serialNumber = 0;

		BaseColliderGroup* group = nullptr;

		template<class T>
		bool TypeCompare(const std::string& type) const
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
		template<class T>
		T* AddCollider(const std::optional<ColliderInfo>& info = std::nullopt)
		{
			// コライダーの型がColliderであるかを確認
			const std::string TYPE_NAME = typeid(T).name();
			auto TypeCompare = [&TYPE_NAME](const std::string& type) { return TYPE_NAME.find(type) != std::string::npos; };
			if (!TypeCompare("Collider")) { return nullptr; }

			// コライダーグループが登録されていない場合は登録する
			std::unique_ptr<BaseSingleCollider> newCollider;
			ColliderInfo colliderInfo(group->GetColliderInfo());
			std::string groupName = info ? info->GetName() : group->GetName();
			colliderInfo.SetName(groupName);

			// コライダー情報が指定されている場合は上書きする
			newCollider = std::make_unique<T>();
			newCollider->Initialize(this, colliderInfo);
			return static_cast<T*>(group->AddCollider(std::move(newCollider)));
		}

		void Initialize(const std::string& groupName, const std::optional<ColliderInfo>& info = std::nullopt);

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

	bool CheckCollisionPair(const BaseCollisionPair& p1, const BaseCollisionPair& p2);
	bool CheckFiltering(const ColliderInfo* infoA, const ColliderInfo* infoB);
}