#pragma once
#include <stdint.h>

enum class CollisionAttribute
{
	Player = 0b1,
	Ball = 0b1 << 1
};

enum class CollisionMask
{
	Player = CollisionAttribute::Ball,
	Ball = CollisionAttribute::Player
};

inline const uint32_t ChangeVal(CollisionAttribute attribute) { return static_cast<uint32_t>(attribute); }
inline const uint32_t ChangeVal(CollisionMask mask) { return static_cast<uint32_t>(mask); }