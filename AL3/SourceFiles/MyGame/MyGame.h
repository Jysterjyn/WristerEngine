#pragma once
#include "Framework.h"
#include "PostEffect.h"
#include <ModelManager.h>
#include <Skydome.h>

// ���̃Q�[���ŗL�̏����N���X
class MyGame : public WristerEngine::Framework
{
	WristerEngine::_3D::ModelManager* modelManager = WristerEngine::_3D::ModelManager::GetInstance();
	std::unique_ptr<WristerEngine::Skydome> skydome;

	// ������(�I�[�o�[���C�h)
	void Initialize();
	// �X�V(�I�[�o�[���C�h)
	void Update();
	// �`��(�I�[�o�[���C�h)
	void Draw();
	// �I������(�I�[�o�[���C�h)
	void Finalize();
};