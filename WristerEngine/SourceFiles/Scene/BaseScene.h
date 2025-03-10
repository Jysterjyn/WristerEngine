#pragma once
#include "Input.h"
#include "DebugCamera.h"
#include "AbstractUIDrawer.h"
#include <ModelManager.h>
#include <memory>

namespace WristerEngine
{
	class SceneManager;

	// �V�[�����N���X
	class BaseScene
	{
	protected:
		SceneManager* sceneManager = nullptr;
		Input* input = Input::GetInstance();
		_3D::DebugCamera debugCamera;
		_3D::ModelManager* modelManager=_3D::ModelManager::GetInstance();
		std::unique_ptr<_2D::AbstractUIDrawer> uiDrawer;

	public:
		// �R���X�g���N�^
		BaseScene();
		virtual ~BaseScene() = default;
		// ������
		virtual void Initialize() = 0;
		// �X�V
		virtual void Update() = 0;
		// �`��
		virtual void Draw() { if (uiDrawer) { uiDrawer->Draw(); } }
		// �I��
		virtual void Finalize() {}
	};
}