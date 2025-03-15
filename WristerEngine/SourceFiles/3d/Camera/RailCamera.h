#pragma once
#include <ModelManager.h>

namespace WristerEngine::_3D
{
	class RailCamera
	{
		ModelManager* modelManager = ModelManager::GetInstance();
		Transform transform;
		Camera camera;

	public:
		void Initialize(const Vector3& pos = {}, const Vector3& rot = {});
		void Update();
		Transform* GetTransform() { return &transform; }
	};
}