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
