#pragma once
#include "Vector.h"

// 4x4行列
class Matrix4
{
public:
	float m[4][4] =
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
	Matrix4 operator*=(const Matrix4& m2);
	// ベクトルの代入
	void SetVector(Vector3 vec, size_t row);
	// 行列からベクトルを得る
	Vector3 GetVector(size_t row) const;
	// 逆行列にする
	void Inverse();

	// 単位行列
	static Matrix4 Identity();
	// ゼロ行列
	static Matrix4 Zero();
	// スケール行列
	static Matrix4 Scale(const Vector3& s);
	// X軸周りの回転行列
	static Matrix4 RotateX(float angle);
	// Y軸周りの回転行列
	static Matrix4 RotateY(float angle);
	// Z軸周りの回転行列
	static Matrix4 RotateZ(float angle);
	// 回転行列
	static Matrix4 Rotate(const Vector3& r);
	// 平行移動行列
	static Matrix4 Translate(const Vector3& t);
};

// 転置行列
Matrix4 Transpose(const Matrix4& mat);
// 逆行列
Matrix4 Inverse(const Matrix4& mat);
// 3Dベクトルから行列を作る
Matrix4 CreateFromVector(const Vector3& vec1 = { 1,0,0 }, const Vector3& vec2 = { 0,1,0 }, const Vector3& vec3 = { 0,0,1 }, const Vector3& vec4 = {});

Matrix4 operator*(const Matrix4& m1, const Matrix4& m2);
Vector3 operator*(const Vector3& v, const Matrix4& m);