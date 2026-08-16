#pragma once
#include <cstdint>
#include <WristerEngineUtility.h>
#include <optional>

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

		virtual ~ColliderInfo() noexcept = default;

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

	template <class T>
	class BaseCollider;

	template <class T>
	class BaseSingleCollider : public ColliderInfo
	{
	protected:
		bool isDestroy = false;
		BaseCollider<T>* owner = nullptr;
		uint32_t serialNumber = 0;
		int shapeType = 0;

	public:
		virtual ~BaseSingleCollider() = default;
		virtual void Update() {}
		void Destroy() { isDestroy = true; }

		void Initialize(BaseCollider<T>* owner_, const ColliderInfo& info)
		{
			owner = owner_;
			SetColliderInfo(info);
		}

		// getter
		bool IsDestroy() const { return isDestroy; }
		BaseCollider<T>* GetOwner() { return owner; }
		uint32_t GetSerialNumber() const { return serialNumber; }
	};

	template <class T>
	struct HitInfo
	{
		std::optional<T> inter = std::nullopt;
		std::optional<float> distance = std::nullopt;
		std::optional<T> reject = std::nullopt;

		HitInfo(const std::optional<T>& inter = std::nullopt, const std::optional<float>& distance = std::nullopt,
			const std::optional<T>& reject = std::nullopt)
			: inter(inter), distance(distance), reject(reject)
		{
		}

		void Reset() { inter = reject = std::nullopt; distance = std::nullopt; }

		virtual ~HitInfo() = default;
	};

	template <class T>
	class BaseCollisionPair : public HitInfo<T>
	{
		BaseSingleCollider<T>* my = nullptr, * other = nullptr;

		BaseCollisionPair(BaseSingleCollider<T>* my_, BaseSingleCollider<T>* other_, const HitInfo<T>& hitInfo)
		{
			my = my_; other = other_; inter = hitInfo.inter; distance = hitInfo.distance; reject = hitInfo.reject;
		}

		static bool Check(const BaseCollisionPair<T>& p1, const BaseCollisionPair<T>& p2)
		{
			// ペアが同じかは、両方のコライダーのシリアルナンバーが同じか、
			// 片方のコライダーのシリアルナンバーがもう片方のコライダーの
			// シリアルナンバーと同じかで判断する
			std::vector<uint32_t> serials1{ p1.my->GetSerialNumber(), p1.other->GetSerialNumber() };
			std::vector<uint32_t> serials2{ p2.my->GetSerialNumber(), p2.other->GetSerialNumber() };

			if (CompareVectors<uint32_t>(serials1, serials2)) { return true; }

			return false;
		}
	};

	template <class T>
	class BaseColliderGroup : public ColliderInfo
	{
	protected:
		uList<BaseSingleCollider<T>> colliders;
		std::list<BaseCollider<T>*> owners;

		// 当たったペアの記録
		std::vector<BaseCollisionPair<T>> collisionPairs;
		std::vector<BaseCollisionPair<T>> enterPairs;
		std::vector<BaseCollisionPair<T>> exitPairs;
		std::vector<BaseCollisionPair<T>> collisionPairsPre;
	
	public:
		virtual ~BaseColliderGroup() noexcept override = default;
		BaseColliderGroup(const std::string& groupName) { SetName(groupName); }
	};

	template <class T>
	class BaseCollider
	{
	protected:
		uint32_t serialNumber = 0;

		BaseColliderGroup<T>* group = nullptr;

		template<class T>
		bool TypeCompare(const std::string& type) const
		{
			const std::string TYPE_NAME = typeid(type).name();
			return TYPE_NAME.find(type) != std::string::npos;
		}

	public:
		// <summary>
		// コライダーを登録
		// </summary>
		// <param name="shapeType">コライダーの形状</param>
		// <returns>登録されたコライダー</returns>
		//template<class T>
		//T* AddCollider(const std::optional<ColliderInfo>& info = std::nullopt)
		//{
		//	std::unique_ptr<BaseCollider> newCollider;
		//	ColliderInfo colliderInfo(group->GetColliderInfo());
		//	std::string groupName = info ? info->GetName() : group->GetName();
		//	colliderInfo.SetName(groupName);
		//
		//	if (TypeCompare("Circle")) { newCollider = std::make_unique<CircleCollider>(); }
		//	else if (TypeCompare("Box")) { newCollider = std::make_unique<BoxCollider>(); }
		//	else if (TypeCompare("TwoRay")) { newCollider = std::make_unique<TwoRayCollider>(); }
		//	if (!newCollider) { return nullptr; }
		//
		//	newCollider->Initialize(this, colliderInfo);
		//
		//	return static_cast<T*>(group->AddCollider(std::move(newCollider)));
		//}
		
		// コライダー生成クラス
		virtual uPtr<BaseCollider> CreateCollider(CR<std::string> type) = 0;

		// 衝突コールバック関数
		// 当たっている間
		virtual void OnCollision() {}
		// 当たった瞬間
		virtual void OnCollisionEnter() {}
		// 離れた瞬間
		virtual void OnCollisionExit() {}

		virtual ~BaseCollider() = default;

		void DeleteGroup() { group = nullptr; }
		// getter
		const std::vector<BaseCollisionPair<T>>& GetCollisionPairs() const { return group->GetCollisionPairs(); }
		BaseColliderGroup<T>* GetGroup() const { return group; }
		uint32_t GetSerialNumber() const { return serialNumber; }
	};

	bool CheckFiltering(const ColliderInfo* infoA, const ColliderInfo* infoB);
}