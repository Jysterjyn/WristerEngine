#pragma once
#include <vector>
#include <cmath>
#include <WristerEngineUtility.h>

const float PI = 3.141592f; // 円周率
const float ONE_DEG_RAD = PI / 180.0f; // 1°のラジアン変換

// 角度
class Angle
{
	float angle = 0;

	// 度数法をラジアンに変換
	float ToRadian(int deg) { return (float)deg * ONE_DEG_RAD; }
	// angleを[-2PI,+2PI]の範囲にする
	void ModAngle() { angle = std::fmod(angle, 2.0f * PI); }

public:
	float operator-() const { return -angle; }
	float operator++(int) { float rAngle = angle; angle += ONE_DEG_RAD; return rAngle; }
	float operator--(int) { float rAngle = angle; angle -= ONE_DEG_RAD; return rAngle; }
	float operator+=(float rad) { angle += rad; ModAngle(); return angle; }
	float operator+=(int deg) { angle += ToRadian(deg); ModAngle(); return angle; }
	float operator-=(float rad) { angle -= rad;  ModAngle(); return angle; }
	float operator-=(int deg) { angle -= ToRadian(deg);  ModAngle(); return angle; }
	operator float() { return angle; }
	Angle() = default;
	Angle(float rad) { angle = rad; ModAngle(); } // ラジアン代入
	Angle(int deg) { angle = ToRadian(deg);  ModAngle(); } // 度数代入
};

Angle operator+(Angle angle, float rad);
Angle operator+(float rad, Angle angle);
Angle operator-(Angle angle, float rad);
Angle operator-(float rad, Angle angle);

// XYZ軸のenum class
enum class Axis { X, Y, Z };

// 数をループさせる関数
template <class T>
T NumberLoop(T num, T max, T min = 0)
{
	if (num > max) { return num - max + min; }
	if (num < min) { return max - min + num; }
	return num;
}

/// <summary>
/// 半分の値を返す
/// </summary>
/// <typeparam name="T">型名</typeparam>
/// <param name="num">値</param>
/// <returns>num / 2の値</returns>
template<class T>
T Half(T num)
{
	return num / 2;
}

/// <summary>
/// 複数値比較
/// </summary>
/// <typeparam name="T">変数型</typeparam>
/// <param name="n">値</param>
/// <param name="list">比較する値の配列</param>
/// <returns>listにnと同値があったらtrue</returns>
template<class T>
bool IsAny(T n, std::vector<T> list)
{
	bool flag = false;
	for (T num : list)
	{
		if (n == num) { flag = true; }
	}
	return flag;
}

// 最短角度補間
float LerpShortAngle(float a, float b, float t);

/// <summary>
/// nがmin~maxの範囲に含まれているか
/// </summary>
/// <param name="n">値</param>
/// <param name="min">最小値</param>
/// <param name="max">最大値</param>
/// <param name="isMinInclude">最低境界値を含むか</param>
/// <param name="isMaxInclude">最大境界値を含むか</param>
/// <returns>nがmin~maxの範囲に含まれていたらtrue</returns>
bool IsInRange(int n, int min, int max, bool isMinInclude, bool isMaxInclude);