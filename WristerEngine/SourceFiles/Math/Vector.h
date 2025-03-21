#pragma once
#include "MathUtility.h"
#include <array>

// 2D�x�N�g��
class Vector2
{
public:
	float x = 0, y = 0;

	// �R���X�g���N�^
	Vector2(float x_ = 0, float y_ = 0) { x = x_, y = y_; }

	// �m����(����)
	float Length() const { return sqrtf(x * x + y * y); }
	// ���K��
	Vector2 Normalize();

	// �P�����Z�q�I�[�o�[���[�h
	Vector2 operator-() const { return Vector2(-x, -y); }

	// ������Z�q�I�[�o�[���[�h
	Vector2& operator+=(const Vector2& v);
	Vector2& operator-=(const Vector2& v);
	Vector2& operator*=(float s);
	Vector2& operator/=(float s);
	float operator[](size_t index);
};

class Matrix4;

// 3D�x�N�g��
class Vector3
{
public:
	float x = 0, y = 0, z = 0;

	// �R���X�g���N�^
	Vector3(float x_ = 0, float y_ = 0, float z_ = 0) { x = x_, y = y_, z = z_; }
	Vector3(const Vector2& v, float z_ = 0) { x = v.x, y = v.y, z = z_; }

	// �m����(����)
	float Length() const { return sqrtf(x * x + y * y + z * z); }
	// ���K��
	Vector3 Normalize();
	// �e�����̐�Βl��Ԃ�
	Vector3 abs();

	// �P�����Z�q�I�[�o�[���[�h
	Vector3 operator-() const { return Vector3(-x, -y, -z); }

	// ������Z�q�I�[�o�[���[�h
	Vector3& operator+=(const Vector3& v);
	Vector3& operator-=(const Vector3& v);
	Vector3& operator*=(float s);
	Vector3& operator*=(const Matrix4& m);
	Vector3& operator/=(float s);

	bool operator<=(Vector3);
	bool operator>=(Vector3);
	bool operator<(Vector3);
	bool operator>(Vector3);
	bool operator==(Vector3);

	// �z��Ƃ��Ĉ�����
	float& operator[](size_t index);

	// �����쐬
	static Vector3 MakeAxis(Axis axis);

	operator Vector2() { return { x,y }; }
};

// 2�����Z�q�I�[�o�[���[�h
const Vector2 operator+(const Vector2& v1, const Vector2& v2);
const Vector2 operator-(const Vector2& v1, const Vector2& v2);
const Vector2 operator*(const Vector2& v, float s);
const Vector2 operator*(float s, const Vector2& v);
const Vector2 operator/(const Vector2& v, float s);
const Vector3 operator+(const Vector3& v1, const Vector3& v2);
const Vector3 operator-(const Vector3& v1, const Vector3& v2);
const Vector3 operator*(const Vector3& v, float s);
const Vector3 operator*(float s, const Vector3& v);
const Vector3 operator/(const Vector3& v, float s);

// ���ϒl
float Dot(const Vector2& v1, const Vector2& v2);
float Dot(const Vector3& v1, const Vector3& v2);
// �O�ϒl
float Cross(const Vector2& v1, const Vector2& v2);
Vector3 Cross(const Vector3& v1, const Vector3& v2);
// �����o�֐��̃O���[�o���֐���
// �m����(����)
float Length(const Vector3& v);
// ���K��
Vector2 Normalize(const Vector2& v);
Vector3 Normalize(const Vector3& v);
// 3D��2D���W
Vector2 To2DVector(const Vector3& vec);

// frontVec�𐳖ʃx�N�g���Ƃ���3�����v�Z����
std::array<Vector3, 3> CalculateAxis(const Vector3& frontVec, const Vector3* up = nullptr);

// �͈͓��̃����_���ȑ傫���̃x�N�g���𐶐�����
Vector2 RandomVector(Vector2 range);
Vector3 RandomVector(Vector3 range);

// ���`���
template <class T>
T Lerp(const T& start, const T& end, const float t) { return start * (1.0f - t) + end * t; }
// �x�W�G�Ȑ�
Vector3 BezierCurve(std::vector<Vector3> p, float t);
// �X�v���C���Ȑ�
Vector3 SplineCurve(const std::vector<Vector3>& points, size_t startIndex, float t);
Vector3 SplineCurve(const std::vector<Vector3>& points, float t);