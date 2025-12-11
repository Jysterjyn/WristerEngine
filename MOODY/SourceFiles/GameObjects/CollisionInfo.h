#pragma once
#include <stdint.h>

enum class CollisionAttribute
{
};

inline uint32_t ChangeVal(CollisionAttribute attribute) { return static_cast<uint32_t>(attribute); }