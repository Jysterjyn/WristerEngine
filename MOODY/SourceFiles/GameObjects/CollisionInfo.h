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