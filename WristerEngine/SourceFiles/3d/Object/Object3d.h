#pragma once
#include "Mesh.h"
#include "Transform.h"

namespace WristerEngine::_3D
{
	// �I�u�W�F�N�g�N���X
	class Object3d
	{
	public:
		Material material;
		Transform transform;
		bool isDestroy = false;
		bool isInvisible = false;

	private:
		friend class ModelManager;
		Mesh* mesh = nullptr;
		/// <summary>
		/// ������
		/// </summary>
		/// <param name="mesh">���b�V���f�[�^</param>
		void Initialize(Mesh* mesh);
		// �X�V
		void Update();
		// �`��
		void Draw();
	};
}