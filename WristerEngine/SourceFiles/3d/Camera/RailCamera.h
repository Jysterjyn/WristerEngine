#pragma once
#include <ModelManager.h>

namespace WristerEngine::_3D
{
	class RailCamera : public BaseCamera
	{
		ModelManager* modelManager = ModelManager::GetInstance();
		Transform transform;

	public:
		RailCamera(const Vector3& pos = {}, const Vector3& rot = {});
		Transform* GetTransform() { return &transform; }

		// BaseCamera ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
		void VirtualUpdate() override;
	};
}