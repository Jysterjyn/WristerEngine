#pragma once
#include "Timer.h"
#include <list>
#include "Transform.h"

namespace WristerEngine
{
	// �g�U����p�[�e�B�N��
	class DiffuseParticle
	{
	public:
		// �p�[�e�B�N��1��
		struct Particle
		{
			Vector3 position; // ���W
			Vector3 velocity; // ���x
			Vector3 accel; // �����x
			FrameTimer frame = 0;
			float s_scale = 1.0f; // �����l
			float e_scale = 0.0f; // �ŏI�l
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
		// �X�V
		void Update();
		// �p�[�e�B�N���폜
		void Clear() { particles.clear(); }
		// �p�[�e�B�N���ǉ�
		void Add(const AddProp& particleProp);
		// �p�[�e�B�N�����X�g�擾
		const std::list<Particle>& GetParticles() const { return particles; }
	};
}