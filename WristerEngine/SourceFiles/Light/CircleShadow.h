#pragma once
#include "Vector.h"
#include "Color.h"
#include <cmath>

namespace WristerEngine
{
	// ŠÛ‰e
	class CircleShadow
	{
	private:
		Vector3 dir = { 1.0f };
		float distanceCasterLight = 100.0f;
		Vector3 casterPos;
		Vector3 atten = { 0.5f,0.6f };
		Vector2 factorAngleCos = { 0.5f,0.2f };
		bool active = false;

	public:
		struct ConstBufferData
		{
			Vector3 dir;
			float pad1;
			Vector3 casterPos;
			float distanceCasterLight;
			Vector3 atten;
			float pad4;
			Vector2 factorAngleCos;
			unsigned int active;
			float pad5;
		};

		// setter
		void SetDir(const Vector3& dir_) { dir = Normalize(dir_); }
		void SetDistanceCasterLight(float distanceCasterLight_) { distanceCasterLight = distanceCasterLight_; }
		void SetCasterPos(const Vector3& casterPos_) { casterPos = casterPos_; }
		void SetAtten(const Vector3& atten_) { atten = atten_; }
		void SetFactorAngle(const Vector2& factorAngle)
		{
			factorAngleCos.x = std::cos(factorAngle.x);
			factorAngleCos.y = std::cos(factorAngle.y);
		}
		void SetActive(bool active_) { active = active_; }
		// getter
		const Vector3& GetDir() const { return dir; }
		float GetDistanceCasterLight() const { return distanceCasterLight; }
		const Vector3& GetCasterPos() const { return casterPos; }
		const Vector3& GetAtten() const { return atten; }
		const Vector2& GetFactorAngleCos() const { return factorAngleCos; }
		bool IsActive() const { return active; }
	};
}