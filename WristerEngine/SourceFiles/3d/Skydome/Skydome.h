#pragma once
#include "Object3d.h"

namespace WristerEngine
{
	// �V���I�u�W�F�N�g
	class Skydome
	{
	private:
		_3D::Object3d* object;

	public:
		// ������
		void Initialize(const std::string& modelName, float scale);
	};
}