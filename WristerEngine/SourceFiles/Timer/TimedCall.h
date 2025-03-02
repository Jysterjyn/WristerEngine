#pragma once
#include <functional>
#include <Timer.h>

namespace WristerEngine
{	// ��������
	class TimedCall
	{
	private:
		// �R�[���o�b�N
		std::function<void(void)> callfunc;
		// �c�莞��
		FrameTimer timer;
		// �����t���O
		bool isFinished = false;

	public:
		// �R���X�g���N�^
		TimedCall(std::function<void(void)>callfunc, uint32_t time);;
		// �X�V
		void Update();
		// �����Ȃ�true��Ԃ�
		bool IsFinished()const { return isFinished; }
	};
}