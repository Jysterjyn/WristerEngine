#include "SceneManager.h"
#include "Sprite.h"
using namespace WristerEngine;

BaseScene::BaseScene() { sceneManager = SceneManager::GetInstance(); }

SceneManager* SceneManager::GetInstance()
{
	static SceneManager sceneManager;
	return &sceneManager;
}

void SceneManager::Initialize(std::unique_ptr<ISceneFactory>& sceneFactory_, const std::string& startScene)
{
	fadeManager.Initialize();
	sceneFactory = move(sceneFactory_);
	nextScene = startScene;
}

void SceneManager::Update()
{
	fadeManager.Update();

	bool isChangeScene = fadeManager.IsChange() || !fadeManager.IsFade();
	isChangeScene &= nextScene.has_value();
	if (isChangeScene)
	{
		// シーン解放
		if (scene)
		{
			scene->Finalize();
			scene.reset();
		}

		scene = sceneFactory->CreateScene(nextScene.value());
		nowScene = nextScene.value();
		nextScene = std::nullopt;

		if (isObjectClear)
		{
			isObjectClear = false;
		}
		if (isParticleClear)
		{
			isParticleClear = false;
		}

		scene->Initialize();
		scene->Update();
		if (pauseMenu) { pauseMenu->Initialize(); }
	}

	if (fadeManager.IsFade()) { return; }
	// ポーズ中ならシーンの更新をせずポーズメニューのみ更新する
	if (pauseMenu) { if (pauseMenu->IsPause()) { pauseMenu->Update(); return; } }
	scene->Update();
}

void SceneManager::Draw()
{
	scene->Draw();
	if (fadeManager.IsFade())
	{
		spMan->PreDraw();
		fadeManager.Draw();
	}
	// ポーズ中ならポーズメニューを描画
	if (pauseMenu) { if (pauseMenu->IsPause()) { pauseMenu->Draw(); } }
}

void SceneManager::ChangeScene(const std::string& nextScene_, bool isObjectClear_, bool isParticleClear_, bool isUseFade)
{
	nextScene = nextScene_;
	isObjectClear = isObjectClear_;
	isParticleClear = isParticleClear_;
	if (isUseFade) { fadeManager.FadeScene(); }
}
