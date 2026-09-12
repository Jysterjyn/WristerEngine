#pragma once
#include "BaseScene.h"
#include "IFadeManager.h"
#include "BasePauseMenu.h"
#include "ISceneFactory.h"
#include <optional>

namespace WristerEngine
{
	struct InitializeSceneManagerParam
	{
		std::unique_ptr<ISceneFactory> sceneFactory;
		std::string startScene;
		uPtr<IFadeManager> fadeManager;
		uPtr<BasePauseMenu> pauseMenu;
	};

	// シーン管理
	class SceneManager final : _2D::SpriteUtility
	{
	private:
		std::unique_ptr<BaseScene> scene;
		std::string nowScene = "None";
		std::optional<std::string> nextScene = std::nullopt;
		uPtr<IFadeManager> fadeManager;
		uPtr<ISceneFactory> sceneFactory;
		bool isObjectClear = false, isParticleClear = false;
		uPtr<BasePauseMenu> pauseMenu;

		SceneManager() = default;
		~SceneManager() = default;
		SceneManager(const SceneManager&) = delete;
		SceneManager& operator=(const SceneManager&) = delete;

	public:
		// インスタンス取得
		static SceneManager* GetInstance();
		// 初期化
		void Initialize(InitializeSceneManagerParam& param);
		// 更新
		void Update();
		// 描画
		void Draw();
		// 終了
		void Finalize() { scene->Finalize(); }
		// ポーズ切り替え
		void Pause() { pauseMenu->Pause(); }
		// シーン切り替え
		void ChangeScene(const std::string& nextScene, bool isObjectClear = true, bool isParticleClear = true, bool isUseFade = true);
		// 現在のシーン取得
		const std::string& GetNowScene() const { return nowScene; }
		// シーンのポインタが存在するか
		bool CheckVariables() const;
	};
}