#pragma once
#include "ModelManager.h"

namespace WristerEngine::_3D
{
	// �I�u�W�F�N�g�ɒǏ]����J����
	class BaseCamera
	{
	protected:
		float distance = 50.0f; // �����_���王�_�܂ł̋���
		Camera camera;
		Vector2 angle = { 0,0.3f }; // �J������]��
		Transform transform;

	public:
		// ���z�f�X�g���N�^
		virtual ~BaseCamera() = default;
		/// <summary>
		/// ������
		/// </summary>
		/// <param name="parent">���[���h�g�����X�t�H�[���̐e�|�C���^</param>
		virtual void Initialize(Transform* parent);
		// �X�V
		virtual void Update();
		// �V�F�C�N�J�n
		void StartShake() { camera.shake->Start(); }
		// getter
		Camera* GetCamera() { return &camera; }
		const Vector2& GetAngle() { return angle; }
	};
}