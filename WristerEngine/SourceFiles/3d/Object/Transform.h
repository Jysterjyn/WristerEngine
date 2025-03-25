#pragma once
#include "Matrix4.h"
#include "WindowsAPI.h"
#include <wrl.h>
#include <d3d12.h>

namespace WristerEngine::_3D
{
	class Object3d;

	// ���[���h�g�����X�t�H�[��
	class Transform
	{
	private:
		// �萔�o�b�t�@�p�f�[�^�\����
		struct ConstBufferData
		{
			Matrix4 world; // ���[���h�s��
		};

		ConstBufferData* constMap = nullptr;
		Microsoft::WRL::ComPtr<ID3D12Resource> constBuffer;
		bool isObject3d = false; // Object3D�̃����o�ϐ���
		bool isUpdated = false;
		friend Object3d;

	public:
		Matrix4 matWorld;
		Vector3 scale = { 1,1,1 }, rotation, translation;
		Transform* parent = nullptr;

		// ������
		void Initialize();
		// �X�V
		void Update();
		// �`��
		void Draw();
		// getter
		Vector3 GetWorldPosition() const { return matWorld.GetVector(3); }
		Vector3 GetScale() const;
	};
}