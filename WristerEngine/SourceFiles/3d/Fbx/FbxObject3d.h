#pragma once
#include "FbxModel.h"
#include "Transform.h"
#include "LightGroup.h"

namespace WristerEngine::_3D
{
	class FbxObject3d
	{
	protected:
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	public:
		static const int MAX_BONES = 32;

		// �萔�o�b�t�@�p�f�[�^�\���́i���W�ϊ��s��p�j
		struct ConstBufferData
		{
			Matrix4 viewproj;    // �r���[�v���W�F�N�V�����s��
			Matrix4 world; // ���[���h�s��
			Vector3 cameraPos; // �J�������W�i���[���h���W�j
		};

		struct ConstBufferDataSkin
		{
			Matrix4 bones[MAX_BONES];
		};

	private: // �ÓI�����o�ϐ�
		static LightGroup* lightGroup;

	protected: // �����o�ϐ�
		// �萔�o�b�t�@
		ComPtr<ID3D12Resource> constBuff, constBuffSkin;
		ConstBufferData* constMap = nullptr;
		ConstBufferDataSkin* constMapSkin = nullptr;
		Transform* transform;
		// ���f��
		FbxModel* model = nullptr;
		FbxTime frameTime, startTime, endTime, currentTime;
		bool isPlay = false;

	public:
		static void SetLightGroup(LightGroup* lightGroup_) { lightGroup = lightGroup_; }

		/// <summary>
		/// ������
		/// </summary>
		void Initialize(Transform* transform, FbxModel* model);

		/// <summary>
		/// ���t���[������
		/// </summary>
		void Update();

		/// <summary>
		/// �`��
		/// </summary>
		void Draw();

		void PlayAnimation();
	};
}