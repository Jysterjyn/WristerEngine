#pragma once
#include "BaseScene.h"
#include "FadeManager.h"
#include "BasePauseMenu.h"
#include "AbstractSceneFactory.h"

namespace WristerEngine
{
	// �V�[���Ǘ�
	class SceneManager final
	{
	private:
		std::unique_ptr<BaseScene> scene;
		std::string nowScene = "None";
		std::string nextScene = "None";
		FadeManager fadeManager;
		std::unique_ptr<AbstractSceneFactory> sceneFactory;
		bool isObjectClear = false, isParticleClear = false;
		BasePauseMenu* pauseMenu = nullptr;

		SceneManager() = default;
		~SceneManager() = default;

	public:
		SceneManager(const SceneManager& obj) = delete;
		SceneManager& operator=(const SceneManager& obj) = delete;
		// �C���X�^���X�擾
		static SceneManager* GetInstance();
		// ������
		void Initialize(std::unique_ptr<AbstractSceneFactory>& sceneFactory, const std::string& startScene);
		// �X�V
		void Update();
		// �`��
		void Draw();
		// �I��
		void Finalize() { scene->Finalize(); }
		// �|�[�Y���j���[���Z�b�g
		void SetPauseMenu(BasePauseMenu* pauseMenu_) { pauseMenu = pauseMenu_; }
		// �|�[�Y�؂�ւ�
		void Pause() { pauseMenu->Pause(); }
		// �V�[���؂�ւ�
		void ChangeScene(const std::string& nextScene, bool isObjectClear = true, bool isParticleClear = true, bool isUseFade = true);
		// ���݂̃V�[���擾
		const std::string& GetNowScene() const { return nowScene; }
	};
}