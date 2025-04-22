#pragma once
#include "BaseCamera.h"
#include "Quaternion.h"

namespace WristerEngine::_3D
{
	// �f�o�b�O�J����
	class DebugCamera : public BaseCamera
	{
	private:
		float distance = 50;
		Quaternion rotQ = 1.0f;
		float mouseMoveDec = 75; // ���s�ړ��̑��x�␳
		float wheelSpdDec = 50; // �Y�[���̑��x�␳

	public:
		struct Prop
		{
			Vector3 targetPos;
			float distance = 50;
			float mouseMoveDec = 75;
			float wheelSpdDec = 50;
		};

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="targetPos">�����_���W</param>
		/// <param name="distance">�����_���W�܂ł̋���</param>
		/// <param name="mouseMoveDec">���s�ړ��̑��x�␳</param>
		/// <param name="wheelSpdDec">�Y�[���̑��x�␳</param>
		DebugCamera(const Prop* prop = nullptr);
		// �X�V
		void VirtualUpdate() override;
	};
}