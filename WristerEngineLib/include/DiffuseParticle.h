#pragma once
#include "Timer.h"
#include <list>
#include "Transform.h"

namespace WristerEngine
{
	// 拡散するパーティクル
	class DiffuseParticle
	{
	public:
		// パーティクル1粒
		struct Particle
		{
			Vector3 position; // 座標
			Vector3 velocity; // 速度
			Vector3 accel; // 加速度
			FrameTimer frame = 0;
			float s_scale = 1.0f; // 初期値
			float e_scale = 0.0f; // 最終値
			_3D::Transform* parent = nullptr;
		};

		struct AddProp
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
			unsigned int addNum = 1;
		};

	private:
		std::list<Particle> particles;

	public:
		// 更新
		void Update();
		// パーティクル削除
		void Clear() { particles.clear(); }
		// パーティクル追加
		void Add(const AddProp& particleProp);
		// パーティクルリスト取得
		const std::list<Particle>& GetParticles() const { return particles; }
	};
}