#include "SceneManager.h"
#include "Sprite.h"
using namespace WristerEngine;

BaseScene::BaseScene() { sceneManager = SceneManager::GetInstance(); }

SceneManager* SceneManager::GetInstance()
{
	static SceneManager sceneManager;
	return &sceneManager;
}

void SceneManager::Initialize(InitializeSceneManagerParam& param)
{
	fadeManager = std::move(param.fadeManager);
	if (fadeManager) { fadeManager->Initialize(); }
	sceneFactory = std::move(param.sceneFactory);
	scene = sceneFactory->CreateScene(param.startScene);
	pauseMenu = std::move(param.pauseMenu);
}

void SceneManager::Update()
{
	fadeManager->Update();

	bool isChangeScene = fadeManager->IsChange() || !fadeManager->IsFade();
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

		scene->Update();
		if (pauseMenu) { pauseMenu->Initialize(); }
	}

	if (fadeManager->IsFade()) { return; }
	// ポーズ中ならシーンの更新をせずポーズメニューのみ更新する
	if (pauseMenu && pauseMenu->IsPause()) { pauseMenu->Update(); return; }
	scene->Update();
}

void SceneManager::Draw()
{
	scene->Draw();
	if (fadeManager->IsFade())
	{
		spMan->PreDraw();
		fadeManager->Draw();
	}
	// ポーズ中ならポーズメニューを描画
	if (pauseMenu && pauseMenu->IsPause()) { pauseMenu->Draw(); }
}

void SceneManager::ChangeScene(const std::string& nextScene_, bool isObjectClear_, bool isParticleClear_, bool isUseFade)
{
	nextScene = nextScene_;
	isObjectClear = isObjectClear_;
	isParticleClear = isParticleClear_;
	if (isUseFade) { fadeManager->FadeScene(); }
}

bool WristerEngine::SceneManager::CheckVariables() const
{
	bool checkFlag = scene != nullptr;
	checkFlag &= sceneFactory != nullptr;
	checkFlag &= fadeManager != nullptr;
	return checkFlag;
}
