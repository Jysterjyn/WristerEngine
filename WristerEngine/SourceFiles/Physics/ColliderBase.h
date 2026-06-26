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

	class BaseSingleCollider : public ColliderInfo
	{

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

	};

	bool CheckFiltering(const ColliderInfo* infoA, const ColliderInfo* infoB);
}