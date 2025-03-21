#pragma once
#include "Matrix4.h"
#include "WindowsAPI.h"
#include "Shake.h"
#include <wrl.h>
#include <d3d12.h>

namespace WristerEngine::_3D
{
	// �J����
	class Camera
	{
	private:
		// �萔�o�b�t�@�p�f�[�^�\����
		struct ConstBufferData
		{
			Matrix4 viewproj; // �r���[�v���W�F�N�V�����s��
			Vector3 cameraPos; // �J�������W(���[���h���W)
		};

	public:
		ConstBufferData* constMap = nullptr;
		Microsoft::WRL::ComPtr<ID3D12Resource> constBuffer;
		Vector3 eye = { 0, 0, -50.0f }, target, up = Vector3::MakeAxis(Axis::Y);
		Angle fovAngleY = 45; // ��p(�x)
		// �A�X�y�N�g��(��{��WIN_SIZE�ɏ���)
		float aspectRatio = WIN_SIZE.x / WIN_SIZE.y;
		float nearZ = 0.1f, farZ = 1000.0f; // �f��͈�
		Matrix4 matView, matProjection, matViewProjection;
		// �V�F�C�N�@�\
		std::unique_ptr<Shake> shake;

		// ������
		void Initialize();
		// �X�V
		void Update();
		// �V�F�C�N�𐶐�����
		void CreateShake(const Shake::Prop& shakeProp);
		// �J�����ړ�
		void CameraMove(const Vector3& move);
		// �r���[�s��ƃv���W�F�N�V�����s����|�����킹���s��
		const Matrix4& GetViewProjectionMatrix() const { return matViewProjection; }
	};
}