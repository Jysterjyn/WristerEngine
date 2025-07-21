#pragma once
#include "Timer.h"
#include <list>
#include "Vector.h"
#include <Particle.h>
#include <stdint.h>

namespace WristerEngine
{
	// 始点から終点へ向かうパーティクル
	class DirectionalParticle : public Particle
	{
	public:
		struct Prop : public BaseParticleProp
		{
			Vector3 start;
			Vector3 end;
			float scale = 1;
			uint16_t splitNum = 1; // 制御点の数
			Angle angle = 0; // {0,1,0}が0ラジアン

			/// <summary>
			/// angleとの角度差
			/// </summary>
			/// <param name="x">マイナスの範囲</param>
			/// <param name="y">プラスの範囲</param>
			Vector2 angleRange;

			float radius = 0;

			Prop() { kind = ParticleKind::Directional; }
		};

	private:
		Vector3 start; // 始点
		Vector3 end; // 終点
		uint16_t splitNum = 1;
		Angle angle = 0;
		float radius = 0;
		FrameTimer frame;
		std::vector<Vector3> controlPoints; // 制御点

		void ComputeControlPoints();

		// Particle を介して継承されました
		void Initialize(const BaseParticleProp& prop) override;
		void Update() override;
	};
}