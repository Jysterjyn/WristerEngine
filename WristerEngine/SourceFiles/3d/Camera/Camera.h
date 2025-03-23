#pragma once
#include "BaseCamera.h"

namespace WristerEngine::_3D
{
	// カメラ
	class Camera : public BaseCamera
	{
	private:
		// 固有の更新処理
		virtual void VirtualUpdate() override;

	public:
		virtual ~Camera() = default;
	};
}