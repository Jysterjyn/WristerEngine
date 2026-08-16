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
		int shapeType = 0;

	public:
		virtual ~BaseSingleCollider() = default;
		virtual void Update() {}
		void Destroy() { isDestroy = true; }
		void Initialize(BaseCollider* owner, const ColliderInfo& info);
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

	};

	class BaseColliderGroup : public ColliderInfo
	{

	};

	class BaseCollider
	{
	private:
		uint32_t serialNumber = 0;

	protected:
		BaseColliderGroup* group = nullptr;

		template<class T>
		bool TypeCompare(const std::string& type) const
		{
			const std::string TYPE_NAME = typeid(type).name();
			return TYPE_NAME.find(type) != std::string::npos;
		}

		/// <summary>
		/// コライダーを登録
		/// </summary>
		/// <param name="shapeType">コライダーの形状</param>
		/// <returns>登録されたコライダー</returns>
		//template<class T>
		//T* AddCollider(const std::optional<ColliderInfo>& info = std::nullopt)
		//{
		//	std::unique_ptr<BaseCollider> newCollider;
		//	ColliderInfo colliderInfo(group->GetColliderInfo());
		//	std::string groupName = info ? info->GetName() : group->GetName();
		//	colliderInfo.SetName(groupName);

		//	if (TypeCompare("Circle")) { newCollider = std::make_unique<CircleCollider>(); }
		//	else if (TypeCompare("Box")) { newCollider = std::make_unique<BoxCollider>(); }
		//	else if (TypeCompare("TwoRay")) { newCollider = std::make_unique<TwoRayCollider>(); }
		//	if (!newCollider) { return nullptr; }

		//	newCollider->Initialize(this, colliderInfo);

		//	return static_cast<T*>(group->AddCollider(std::move(newCollider)));
		//}


		// コライダー生成クラス
		virtual uPtr<BaseCollider> CreateCollider(CR<std::string> type) = 0;
	};

	bool CheckFiltering(const ColliderInfo* infoA, const ColliderInfo* infoB);
}