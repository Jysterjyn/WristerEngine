#pragma once
#include <ModelManager.h>

namespace WristerEngine
{
	class AxisIndicator
	{
		_3D::Object3d* obj = nullptr;
		_3D::BaseCamera camera;
		_3D::ModelManager* modelManager = _3D::ModelManager::GetInstance();

		void Initialize();
		void Update();
		void Draw();
	};
}