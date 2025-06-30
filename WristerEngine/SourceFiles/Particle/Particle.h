#pragma once
#include <Vector.h>
#include <Color.h>

namespace WristerEngine
{
	enum class ParticleKind
	{
		Diffuse, Directional
	};

	struct BaseParticleProp
	{
		ParticleKind kind = ParticleKind::Diffuse;
		int lifeTime = 60;
	};

	class Particle
	{
	private:
		bool isDestroy = false;

	protected:
		Vector3 position; // 座標
		float scale = 0;
		ColorRGBA color;

	public:
		virtual ~Particle() = default;
		virtual void Initialize(const BaseParticleProp& prop) = 0;
		virtual void Update() = 0;
		void Destroy() { isDestroy = true; }
		virtual const Vector3 GetPosition() const { return position; }
		float GetScale() const { return scale; }
		const ColorRGBA& GetColor() const { return color; }
		bool IsDestroy() const { return isDestroy; }
	};
}