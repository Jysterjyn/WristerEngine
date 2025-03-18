#pragma once
#include "Framework.h"
#include "PostEffect.h"
#include <ModelManager.h>
#include <PrimitiveDrawer.h>

// ���̃Q�[���ŗL�̏����N���X
class MyGame : public WristerEngine::Framework
{
	WristerEngine::_3D::ModelManager* modelManager = WristerEngine::_3D::ModelManager::GetInstance();
	WristerEngine::_3D::PrimitiveDrawer* primitiveDrawer = WristerEngine::_3D::PrimitiveDrawer::GetInstance();

	// ������(�I�[�o�[���C�h)
	void Initialize();
	// �X�V(�I�[�o�[���C�h)
	void Update();
	// �`��(�I�[�o�[���C�h)
	void Draw();
	// �I������(�I�[�o�[���C�h)
	void Finalize();
};