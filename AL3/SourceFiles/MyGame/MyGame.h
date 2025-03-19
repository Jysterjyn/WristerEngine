#pragma once
#include "Framework.h"
#include <ModelManager.h>
#include <PrimitiveDrawer.h>

// ���̃Q�[���ŗL�̏����N���X
class MyGame : public WristerEngine::Framework
{
	WristerEngine::_3D::ModelManager* modelManager = WristerEngine::_3D::ModelManager::GetInstance();
	WristerEngine::_3D::PrimitiveDrawer* primitiveDrawer = WristerEngine::_3D::PrimitiveDrawer::GetInstance();

	// �p�����ꂽ�֐�
	void Initialize() override;
	void Update() override;
	void Draw() override;
	bool IsEndRequest() override;
};