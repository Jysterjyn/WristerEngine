#pragma once
#include <cstdint>

namespace WristerEngine
{
	struct BaseInfo
	{
	protected:
		uint32_t attribute = 0;
		uint32_t mask = UINT32_MAX;

	public:
		virtual ~BaseInfo() = default;

		// setter
		void SetAttribute(uint32_t attribute_) { attribute = attribute_; }
		void SetMask(uint32_t mask_) { mask = mask_; }
		void SetGroupInfo(const BaseInfo& info)
		{
			attribute = info.attribute;
			mask = info.mask;
		}
		// getter
		uint32_t GetAttribute() const { return attribute; }
		uint32_t GetMask() const { return mask; }
	};

	struct ColliderInfo : public BaseInfo
	{
	protected:
		std::string name;

	public:
		ColliderInfo() = default;
		ColliderInfo(uint32_t attribute_, uint32_t mask_, const std::string& name_ = "")
		{
			attribute = attribute_;
			mask = mask_;
			name = name_;
		}

		// setter
		void SetName(const std::string& name_) { name = name_; }
		void SetColliderInfo(const ColliderInfo& info)
		{
			attribute = info.attribute;
			mask = info.mask;
			name = info.name;
		}
		// getter
		const std::string& GetName() const { return name; }
	};

	static bool CheckFiltering(const BaseInfo* infoA, const BaseInfo* infoB)
	{
		return
			infoA->GetAttribute() & infoB->GetMask() &&
			infoB->GetAttribute() & infoA->GetMask();
	}
}