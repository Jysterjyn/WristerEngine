#include "CollisionInfo.h"

using namespace WE;

ColliderInfo::ColliderInfo(uint32_t attribute_, uint32_t mask_)
{
	attribute = attribute_;
	mask = mask_;
}

ColliderInfo::ColliderInfo(const ColliderInfo& info)
{
	attribute = info.attribute;
	mask = info.mask;
	name = info.name.Get();
}

void ColliderInfo::SetColliderInfo(const ColliderInfo& info)
{
	attribute = info.attribute;
	mask = info.mask;
	name = info.name.Get();
}

ColliderInfo ColliderInfo::GetColliderInfo() const
{
	ColliderInfo info{};
	info.attribute = attribute;
	info.mask = mask;
	return info;
}

bool WE::CheckFiltering(const ColliderInfo* infoA, const ColliderInfo* infoB)
{
	return
		infoA->GetAttribute() & infoB->GetMask() &&
		infoB->GetAttribute() & infoA->GetMask();
}