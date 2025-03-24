#pragma once
#include "Matrix4.h"
#include "WindowsAPI.h"
#include "Shake.h"
#include <Transform.h>
#include <wrl.h>
#include <d3d12.h>

namespace WristerEngine::_3D
{
	// �J�������N���X
	class BaseCamera
	{
	private:
		// �萔�o�b�t�@�p�f�[�^�\����
		struct ConstBufferData
		{
			Matrix4 viewproj; // �r���[�v���W�F�N�V�����s��
			Vector3 cameraPos; // �J�������W(���[���h���W)
		};

		// ���[���h�s����g�����v�Z���s�����߂�Transform�|�C���^
		const Transform* pTransform = nullptr;
		Matrix4 matView, matProjection, matViewProjection;

		/// <summary>
		/// �r���[�s����X�V
		/// </summary>
		/// <returns>�V�F�C�N�l���܂񂾃J�����̍��W</returns>
		Vector3 UpdateViewMatrix();

		// �v���W�F�N�V�����s����X�V
		void UpdateProjectionMatrix();

	public:
		ConstBufferData* constMap = nullptr;
		Microsoft::WRL::ComPtr<ID3D12Resource> constBuffer;
		Vector3 eye = { 0, 0, -50.0f }, target, up = Vector3::MakeAxis(Axis::Y);
		Angle fovAngleY = 45; // ��p(�x)
		// �A�X�y�N�g��(��{��WIN_SIZE�ɏ���)
		float aspectRatio = WIN_SIZE.x / WIN_SIZE.y;
		float nearZ = 0.1f, farZ = 1000.0f; // �f��͈�
		// �V�F�C�N�@�\
		std::unique_ptr<Shake> shake;

		// ���z�f�X�g���N�^
		virtual ~BaseCamera() = default;
		// ������
		void Initialize();
		// �X�V
		virtual void Update();
		// �ŗL�̍X�V����
		virtual void VirtualUpdate() {}
		// �V�F�C�N�𐶐�����
		void CreateShake(const Shake::Prop& shakeProp);
		// �J�����ړ�
		void CameraMove(const Vector3& move);
		// ���[���h�s����Z�b�g
		void SetTransform(const Transform* transform) { pTransform = transform; }
		// �J�����ɑ΂��Ẵr���{�[�h�s���Ԃ�
		Matrix4 GetBillboard() const;
		// ���[���h�s����Z�b�g
		const Transform* GetTransform() const { return pTransform; }
		// �r���[�s��ƃv���W�F�N�V�����s����|�����킹���s��
		const Matrix4& GetViewProjectionMatrix() const { return matViewProjection; }
	};
}