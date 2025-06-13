#pragma once
#include <stdint.h>

enum class CollisionAttribute
{
	Sphere, Box, Plane, Ray
};

uint32_t ChangeVal(CollisionAttribute attribute) { return static_cast<uint32_t>(attribute); }