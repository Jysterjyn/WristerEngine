#pragma once
#include "BaseCamera.h"

namespace WristerEngine::_3D
{
	// �J����
	class Camera : public BaseCamera
	{
	public:
		virtual ~Camera() = default;

		// �ŗL�̍X�V����
		virtual void VirtualUpdate();
	};
}