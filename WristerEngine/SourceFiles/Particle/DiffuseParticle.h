#pragma once
#include "Timer.h"
#include <list>
#include "Transform.h"
#include <Particle.h>
#include <Physics.h>

namespace WristerEngine
{
	// 拡散するパーティクル
	class DiffuseParticle : public Particle
	{
	public:
		struct Prop : public BaseParticleProp
		{
			_3D::Transform* parent = nullptr;
			PosVelAcc offset;
			PosVelAcc range;

			/// <summary>
			/// スケール
			/// </summary>
			/// <param name="x">最初のスケール</param>
			/// <param name="y">最後のスケール</param>
			Vector2 scales = { 1.0f,0.0f };

			Prop();
		};

	private:
		Vector3 velocity; // 速度
		Vector3 accel; // 加速度
		FrameTimer frame = 0;
		Vector2 scales;
		_3D::Transform* parent = nullptr;

		// Particle を介して継承されました
		void Initialize(const BaseParticleProp& prop) override;
		void Update() override;
		const Vector3 GetPosition() const override;
	};

}