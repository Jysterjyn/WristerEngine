#pragma once
#include "Timer.h"
#include <list>
#include "Vector.h"
#include <Particle.h>

namespace WristerEngine
{
	// 始点から終点へ向かうパーティクル
	class DirectionalParticle : public Particle
	{
	public:
		Vector3 start; // 始点
		Vector3 end; // 終点
		unsigned short splitNum = 1;
		float angle = 0;
		float radius = 0;
		FrameTimer frame;
		std::vector<Vector3> controlPoints; // 制御点

		void ComputeControlPoints();

		struct Prop : public BaseParticleProp
		{
			Vector3 start;
			Vector3 end;
			float scale = 1;
			unsigned short splitNum = 1; // 制御点の数
			float angle = 0; // {0,1,0}が0ラジアン
			Vector2 angleRange;
			float radius = 0;
			int lifeTime = 60;
		
			Prop() { kind = ParticleKind::Directional; }
		};

	public:
		// Particle を介して継承されました
		void Initialize(const BaseParticleProp& prop) override;
		void Update() override;
	};
}