#pragma once
#include "Transform.h"
#include "Timer.h"
#include <list>

namespace WristerEngine
{
	// 指定したオブジェクトに追尾するパーティクル
	class TrackParticle
	{
	public:
		// パーティクル1粒
		struct Particle
		{
			Vector3 parentPrePos; // 親座標
			Vector3 position{}; // 座標
			Vector3 velocity{}; // 速度
			Vector3 accel{}; // 加速度
			FrameTimer frame = 0;
			float scale = 1.0f; // スケール
			float s_scale = 1.0f; // 初期値
			float e_scale = 0.0f;	// 最終値
			_3D::Transform* parent = nullptr;

			void Update();
		};

		struct AddProp
		{
			_3D::Transform* parent = nullptr;
			Vector3 posOffset;
			Vector3 velOffset;
			Vector3 accOffset;
			float posRange = 1.0f;
			float velRange = 0.1f;
			float accRange = 0.001f;
			int lifeTime = 60;
			float start_scale = 1.0f;
			float end_scale = 0.0f;
			unsigned short addNum = 1;
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