#pragma once
#include "Timer.h"
#include <list>
#include "Transform.h"
#include <Particle.h>

namespace WristerEngine
{
	// 拡散するパーティクル
	class DiffuseParticle : public Particle
	{
	public:
		struct Prop : public BaseParticleProp
		{
			_3D::Transform* parent = nullptr;
			Vector3 posOffset;
			Vector3 velOffset;
			Vector3 accOffset;
			Vector3 posRange = { 1,1,1 };
			Vector3 velRange = { 0.1f,0.1f,0.1f };
			Vector3 accRange = { 0.001f,0.001f,0.001f };
			int lifeTime = 60;
			float startScale = 1.0f;
			float endScale = 0.0f;

			Prop() { kind = ParticleKind::Diffuse; }
		};

	private:
		Vector3 velocity; // 速度
		Vector3 accel; // 加速度
		FrameTimer frame = 0;
		float s_scale = 0; // 初期値
		float e_scale = 0; // 最終値
		_3D::Transform* parent = nullptr;

		// Particle を介して継承されました
		void Initialize(const BaseParticleProp& prop) override;
		void Update() override;
	};

}