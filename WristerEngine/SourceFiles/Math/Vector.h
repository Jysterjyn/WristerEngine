#pragma once
#include "MathUtility.h"
#include <array>

// 2Dベクトル
class Vector2
{
public:
	float x = 0, y = 0;

	// コンストラクタ
	Vector2(float x_ = 0, float y_ = 0) { x = x_, y = y_; }

	// ノルム(長さ)
	float Length() const { return sqrtf(x * x + y * y); }
	// 正規化
	Vector2 Normalize();

	// 単項演算子オーバーロード
	Vector2 operator-() const { return Vector2(-x, -y); }

	// 代入演算子オーバーロード
	Vector2& operator+=(WE::CR<Vector2> v);
	Vector2& operator-=(WE::CR<Vector2> v);
	Vector2& operator*=(float s);
	Vector2& operator/=(float s);

	// 比較演算子オーバーロード
	bool operator<=(WE::CR<Vector2> v) const;
	bool operator>=(WE::CR<Vector2> v) const;
	bool operator<(WE::CR<Vector2> v) const;
	bool operator>(WE::CR<Vector2> v) const;
	bool operator==(WE::CR<Vector2> v) const;

	// 配列として扱える
	float& operator[](size_t index);
};

class Matrix4;

// 3Dベクトル
class Vector3
{
public:
	float x = 0, y = 0, z = 0;

	// コンストラクタ
	Vector3(float x_ = 0, float y_ = 0, float z_ = 0) { x = x_, y = y_, z = z_; }
	Vector3(WE::CR<Vector2> v, float z_ = 0) { x = v.x, y = v.y, z = z_; }

	// ノルム(長さ)
	float Length() const { return sqrtf(x * x + y * y + z * z); }
	// 正規化
	Vector3 Normalize();
	// 各成分の絶対値を返す
	Vector3 abs();

	// 単項演算子オーバーロード
	Vector3 operator-() const { return Vector3(-x, -y, -z); }

	// 代入演算子オーバーロード
	Vector3& operator+=(const Vector3& v);
	Vector3& operator-=(const Vector3& v);
	Vector3& operator*=(float s);
	Vector3& operator*=(const Matrix4& m);
	Vector3& operator/=(float s);

	// 比較演算子オーバーロード
	bool operator<=(const Vector3& v) const;
	bool operator>=(const Vector3& v) const;
	bool operator<(const Vector3& v) const;
	bool operator>(const Vector3& v) const;
	bool operator==(const Vector3& v) const;

	// 配列として扱える
	float& operator[](size_t index);

	operator Vector2() { return { x,y }; }
};

// 軸ベクトル
static const Vector2 AXIS_X_2D = { 1, 0 };
static const Vector2 AXIS_Y_2D = { 0, 1 };
static const Vector3 AXIS_X_3D = { 1, 0, 0 };
static const Vector3 AXIS_Y_3D = { 0, 1, 0 };
static const Vector3 AXIS_Z_3D = { 0, 0, 1 };

// 2項演算子オーバーロード
const Vector2 operator+(WE::CR<Vector2> v1, WE::CR<Vector2> v2);
const Vector2 operator-(WE::CR<Vector2> v1, WE::CR<Vector2> v2);
const Vector2 operator*(WE::CR<Vector2> v, float s);
const Vector2 operator*(float s, WE::CR<Vector2> v);
const Vector2 operator/(WE::CR<Vector2> v, float s);
const Vector3 operator+(const Vector3& v1, const Vector3& v2);
const Vector3 operator-(const Vector3& v1, const Vector3& v2);
const Vector3 operator*(const Vector3& v, float s);
const Vector3 operator*(float s, const Vector3& v);
const Vector3 operator/(const Vector3& v, float s);

// 内積値
float Dot(WE::CR<Vector2> v1, WE::CR<Vector2> v2);
float Dot(const Vector3& v1, const Vector3& v2);
// 外積値
float Cross(WE::CR<Vector2> v1, WE::CR<Vector2> v2);
Vector3 Cross(const Vector3& v1, const Vector3& v2);
// メンバ関数のグローバル関数版
// ノルム(長さ)
float Length(const Vector3& v);
// 正規化
Vector2 Normalize(WE::CR<Vector2> v);
Vector3 Normalize(const Vector3& v);

// forwardを正面ベクトルとする3軸を計算する
std::array<Vector3, 3> CalculateAxis(const Vector3& forward, const Vector3* up = nullptr);

// 範囲内のランダムな大きさのベクトルを生成する
Vector2 RandomVector(WE::CR<Vector2> range);
Vector3 RandomVector(Vector3 range);

// 線形補間
template <class T>
T Lerp(const T& start, const T& end, const float t) { return start * (1.0f - t) + end * t; }
// ベジエ曲線
Vector3 BezierCurve(std::vector<Vector3> p, float t);
// スプライン曲線
Vector3 SplineCurve(const std::vector<Vector3>& points, size_t startIndex, float t);
Vector3 SplineCurve(const std::vector<Vector3>& points, float t);