#pragma once
#include "BaseCamera.h"

namespace WristerEngine::_3D
{
	// �J����
	class Camera : public BaseCamera
	{
	private:
		// �ŗL�̍X�V����
		virtual void VirtualUpdate() override;

	public:
		virtual ~Camera() = default;
	};
}