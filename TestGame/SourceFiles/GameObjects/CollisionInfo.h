#pragma once
#include <stdint.h>

enum class CollisionAttribute
{
	Sphere = 0b1,
	Plane = 0b1 << 1,
	Box = 0b1 << 2,
	Triangle = 0b1 << 3,
	Ray = 0b1 << 4,
	Allies = 0b1 << 5,
	Landshape = 0b1 << 6 // 地形
};

inline uint32_t ChangeVal(CollisionAttribute attribute) { return static_cast<uint32_t>(attribute); }