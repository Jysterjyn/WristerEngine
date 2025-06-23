#include "MathUtility.h"
#include <cmath>

int NumberLoop(int num, int max, int min)
{
	if (num > max) { return num - max + min; }
	if (num < min) { return max; }
	return num;
}

Angle operator+(Angle angle, float rad)
{
	return angle += rad;
}

Angle operator+(float rad, Angle angle)
{
	return angle += rad;
}

Angle operator-(Angle angle, float rad)
{
	return angle -= rad;
}

Angle operator-(float rad, Angle angle)
{
	return angle -= rad;
}

float LerpShortAngle(float a, float b, float t)
{
	// Šp“x·•ª‚ð‹‚ß‚é
	float diff = b - a;
	// Šp“x‚ð•â³‚·‚é
	diff = std::fmod(diff, 2 * PI); // [-2PI,+2PI]

	if (diff > PI) { diff -= 2 * PI; }
	else if (diff < -PI) { diff += 2 * PI; } // Å’ZŠp“x‚É‚·‚é

	return std::lerp(a, a + diff, t);
}
