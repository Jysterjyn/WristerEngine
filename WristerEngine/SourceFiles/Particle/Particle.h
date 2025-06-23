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
		ParticleKind type = ParticleKind::Diffuse;
	};

	class Particle
	{
	public:
		Vector3 position; // ç¿ïW
		float scale = 0;
		ColorRGBA color;

	public:
		virtual ~Particle() = default;
		virtual void Initialize(const BaseParticleProp& prop) = 0;
		virtual void Update() = 0;
	};
}