#pragma once
#include <cstdint>

namespace WristerEngine
{
	struct CollisionInfo
	{
	protected:
		uint32_t attribute = 0;
		uint32_t mask = UINT32_MAX;

	public:
		CollisionInfo(const uint32_t& attribute = 0, const uint32_t& mask = UINT32_MAX)
			: attribute(attribute), mask(mask) {
		}

		// setter
		void SetAttribute(uint32_t attribute_) { attribute = attribute_; }
		void SetMask(uint32_t mask_) { mask = mask_; }
		// getter
		uint32_t GetAttribute() const { return attribute; }
		uint32_t GetMask() const { return mask; }
	};

	static bool CheckFiltering(const CollisionInfo* infoA, const CollisionInfo* infoB)
	{
		return
			infoA->GetAttribute() & infoB->GetMask() &&
			infoB->GetAttribute() & infoA->GetMask();
	}
}