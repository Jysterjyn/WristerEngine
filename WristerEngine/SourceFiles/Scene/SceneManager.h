#pragma once
#include "BaseScene.h"
#include "FadeManager.h"
#include "BasePauseMenu.h"
#include "AbstractSceneFactory.h"

namespace WristerEngine
{
	// シーン管理
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
		// インスタンス取得
		static SceneManager* GetInstance();
		// 初期化
		void Initialize(std::unique_ptr<AbstractSceneFactory>& sceneFactory, const std::string& startScene);
		// 更新
		void Update();
		// 描画
		void Draw();
		// 終了
		void Finalize() { scene->Finalize(); }
		// ポーズメニューをセット
		void SetPauseMenu(BasePauseMenu* pauseMenu_) { pauseMenu = pauseMenu_; }
		// ポーズ切り替え
		void Pause() { pauseMenu->Pause(); }
		// シーン切り替え
		void ChangeScene(const std::string& nextScene, bool isObjectClear = true, bool isParticleClear = true, bool isUseFade = true);
		// 現在のシーン取得
		const std::string& GetNowScene() const { return nowScene; }
	};
}