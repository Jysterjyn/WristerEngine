#include "ModelManager.h"
#include "DirectXCommon.h"
#include <cmath>
#include <Random.h>

Vector2 Vector2::Normalize()
{
	float len = Length();
	if (len != 0) { *this /= len; }
	return *this;
}

Vector2& Vector2::operator+=(const Vector2& v)
{
	x += v.x;
	y += v.y;
	return *this;
}

Vector2& Vector2::operator-=(const Vector2& v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}

Vector2& Vector2::operator*=(float s)
{
	x *= s;
	y *= s;
	return *this;
}

Vector2& Vector2::operator/=(float s)
{
	x /= s;
	y /= s;
	return *this;
}

float& Vector2::operator[](size_t index)
{
	switch (index)
	{
	case (size_t)Axis::X: return x;
	case (size_t)Axis::Y: return y;
	default: assert(0); return x;
	}
}

const Vector2 operator+(const Vector2& v1, const Vector2& v2)
{
	Vector2 temp(v1);
	return temp += v2;
}

const Vector2 operator-(const Vector2& v1, const Vector2& v2)
{
	Vector2 temp(v1);
	return temp -= v2;
}

const Vector2 operator*(const Vector2& v, float s)
{
	Vector2 temp(v);
	return temp *= s;
}

const Vector2 operator*(float s, const Vector2& v) { return v * s; }

const Vector2 operator/(const Vector2& v, float s)
{
	Vector2 temp(v);
	return temp /= s;
}

Vector3 Vector3::Normalize()
{
	float len = Length();
	if (len != 0) { *this /= len; }
	return *this;
}

Vector3 Vector3::abs()
{
	Vector3 ans = *this;

	ans.x = fabsf(ans.x);
	ans.y = fabsf(ans.y);
	ans.z = fabsf(ans.z);

	return ans;
}

Vector3& Vector3::operator+=(const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vector3& Vector3::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

Vector3& Vector3::operator*=(const Matrix4& m)
{
	Vector3 temp = *this * m;
	*this = temp;
	return *this;
}

Vector3& Vector3::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

bool Vector3::operator<=(Vector3 vec) const
{
	if (x <= vec.x && y <= vec.y && z <= vec.z) { return true; }
	return false;
}

bool Vector3::operator>=(Vector3 vec) const
{
	if (x >= vec.x && y >= vec.y && z >= vec.z) { return true; }
	return false;
}

bool Vector3::operator<(Vector3 vec) const
{
	if (x < vec.x && y < vec.y && z < vec.z) { return true; }
	return false;
}

bool Vector3::operator>(Vector3 vec) const
{
	if (x > vec.x && y > vec.y && z > vec.z) { return true; }
	return false;
}

bool Vector3::operator==(Vector3 vec) const
{
	if (x == vec.x && y == vec.y && z == vec.z) { return true; }
	return false;
}

float& Vector3::operator[](size_t index)
{
	switch (index)
	{
	case (size_t)Axis::X: return x;
	case (size_t)Axis::Y: return y;
	case (size_t)Axis::Z: return z;
	default: assert(0); return z;
	}
}

Vector3 Vector3::MakeAxis(Axis axis)
{
	switch (axis)
	{
	case Axis::X: return { 1,0,0 };
	case Axis::Y: return { 0,1,0 };
	case Axis::Z: return { 0,0,1 };
	}
	return { 0,0,0 };
}

const Vector3 operator+(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	return temp += v2;
}

const Vector3 operator-(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	return temp -= v2;
}

const Vector3 operator*(const Vector3& v, float s)
{
	Vector3 temp(v);
	return temp *= s;
}

const Vector3 operator*(float s, const Vector3& v) { return v * s; }

const Vector3 operator/(const Vector3& v, float s)
{
	Vector3 temp(v);
	return temp /= s;
}

float Dot(const Vector2& v1, const Vector2& v2) { return v1.x * v2.x + v1.y * v2.y; }

float Dot(const Vector3& v1, const Vector3& v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }

float Cross(const Vector2& v1, const Vector2& v2) { return v1.x * v2.y - v1.y * v2.x; }

Vector3 Cross(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp;

	temp =
	{
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	};

	return temp;
}

float Length(const Vector3& v) { return Vector3(v).Length(); }

Vector2 Normalize(const Vector2& v) { return Vector2(v).Normalize(); }

Vector3 Normalize(const Vector3& v) { return Vector3(v).Normalize(); }

static Matrix4 GetViewProjectionViewportMatrix()
{
	Matrix4 matVP = WristerEngine::_3D::ModelManager::GetInstance()->GetCamera()->GetViewProjectionMatrix();
	Matrix4 matV = WristerEngine::DirectXCommon::GetInstance()->GetViewportMatrix();
	return matVP * matV;
}

Vector2 To2DVector(const Vector3& vec)
{
	return vec * GetViewProjectionViewportMatrix();
}

Vector3 To3DVector(const Vector2& vec, float distance)
{
	// 合成行列の逆行列を計算する
	Matrix4 matInverseVPV = Inverse(GetViewProjectionViewportMatrix());
	// スクリーン座標
	Vector3 posNear(vec, 0);
	Vector3 posFar(vec, 1);
	// スクリーン座標系からワールド座標系へ
	posNear *= matInverseVPV;
	posFar *= matInverseVPV;
	// レイの方向
	Vector3 direction = Normalize(posFar - posNear);
	return posNear + direction * distance;
}

std::array<Vector3, 3> CalculateAxis(const Vector3& forward, const Vector3* up)
{
	assert(forward.Length() != 0);

	Vector3 upVec;
	if (up) { upVec = Normalize(*up); }
	else { upVec = Vector3::MakeAxis(Axis::Y); }

	std::array<Vector3, 3> axis;
	axis[(int)Axis::Z] = Normalize(forward);
	axis[(int)Axis::X] = Normalize(Cross(upVec, axis[(int)Axis::Z]));
	axis[(int)Axis::Y] = Normalize(Cross(axis[(int)Axis::Z], axis[(int)Axis::X]));
	return axis;
}

Vector2 RandomVector(Vector2 range)
{
	WristerEngine::Random_Float randPosX(-range.x, range.x);
	WristerEngine::Random_Float randPosY(-range.y, range.y);
	return Vector2(randPosX(), randPosY());
}

Vector3 RandomVector(Vector3 range)
{
	WristerEngine::Random_Float randPosX(-range.x, range.x);
	WristerEngine::Random_Float randPosY(-range.y, range.y);
	WristerEngine::Random_Float randPosZ(-range.z, range.z);
	return Vector3(randPosX(), randPosY(), randPosZ());
}

Vector3 BezierCurve(std::vector<Vector3> p, float t)
{
	assert(p.size() >= 2);
	// 制御点2つのときは線形補間
	if (p.size() == 2) { return Lerp(p[0], p[1], t); }

	std::vector<Vector3> controlPoints;
	for (size_t i = 0; i < p.size() - 1; i++) { controlPoints.push_back(Lerp(p[i], p[i + 1], t)); }
	while (controlPoints.size() != 2)
	{
		std::vector<Vector3> points;
		for (size_t i = 0; i < controlPoints.size() - 1; i++)
		{
			points.push_back(Lerp(controlPoints[i], controlPoints[i + 1], t));
		}
		controlPoints = points;
	}
	return Lerp(controlPoints[0], controlPoints[1], t);
}

Vector3 SplineCurve(const std::vector<Vector3>& points, size_t startIndex, float t)
{
	// 最初と最後に制御点を追加
	std::vector<Vector3> newPoints = points;
	newPoints.insert(newPoints.begin(), points[0]);
	newPoints.push_back(points.back());

	size_t n = newPoints.size() - 2;

	if (startIndex >= n) { return newPoints[n]; }
	if (startIndex < 1) { return newPoints[1]; }

	std::vector<Vector3> p =
	{
		newPoints[startIndex - 1],
		newPoints[startIndex],
		newPoints[startIndex + 1],
		newPoints[startIndex + 2],
	};

	return 0.5f * (
		2 * p[1] + (-p[0] + p[2]) * t +
		(2 * p[0] - 5 * p[1] + 4 * p[2] - p[3]) * t * t +
		(-p[0] + 3 * p[1] - 3 * p[2] + p[3]) * t * t * t);
}

Vector3 SplineCurve(const std::vector<Vector3>& points, float t)
{
	if (t == 0) { return SplineCurve(points, 0, t); }
	if (t >= 1) { return SplineCurve(points, points.size() - 1, 1); }
	float length = float(points.size() - 1);
	float dt = 1.0f / length;
	int index = int((t / dt)) + 1;
	return SplineCurve(points, index, fmodf(t, dt) * length);
}
