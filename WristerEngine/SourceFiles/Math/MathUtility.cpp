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
	// 角度差分を求める
	float diff = b - a;
	// 角度を補正する
	diff = std::fmod(diff, 2 * PI); // [-2PI,+2PI]

	if (diff > PI) { diff -= 2 * PI; }
	else if (diff < -PI) { diff += 2 * PI; } // 最短角度にする

	return std::lerp(a, a + diff, t);
}

bool IsInRange(int n, int min, int max, bool isMinInclude, bool isMaxInclude)
{
	bool flag = false;

	// 最小値の比較
	if (isMinInclude) { flag = n >= min; }
	else { flag = n > min; }

	// 範囲外リターン
	if (!flag) { return false; }

	// 最大値の比較
	if (isMaxInclude) { flag = n <= max; }
	else { flag = n < max; }

	// 範囲外リターン
	if (!flag) { return false; }

	// 全てtrueなら範囲内
	return true;
}
