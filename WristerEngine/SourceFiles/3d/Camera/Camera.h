#pragma once
#include "BaseCamera.h"

namespace WristerEngine::_3D
{
	// カメラ
	class Camera : public BaseCamera
	{
	public:
		virtual ~Camera() = default;

		// 固有の更新処理
		virtual void VirtualUpdate();
	};
}