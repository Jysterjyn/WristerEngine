#pragma once

// コライダーの属性
enum class CollisionAttribute : uint32_t
{
	None = 0,
	Player = 0b1,
	PlayerWeapon = 0b1 << 1,
	Enemy = 0b1 << 2,
};

// コライダーのマスク
enum class CollisionMask : uint32_t
{
	None = 0,
	All = static_cast<uint32_t>(-1),
};

inline uint32_t ChangeVal(CollisionAttribute attribute) { return static_cast<uint32_t>(attribute); }
inline uint32_t ChangeVal(CollisionMask mask) { return static_cast<uint32_t>(mask); }