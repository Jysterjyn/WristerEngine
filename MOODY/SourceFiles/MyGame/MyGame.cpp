#include "MyGame.h"
#include "SceneFactory.h"
#include <ImGuiManager.h>

void MyGame::Initialize()
{
	// フレームワーク初期化
	windowName = L"WristerEngine";
	Framework::Initialize();

	// シーンの生成
	std::unique_ptr<WE::AbstractSceneFactory> sceneFactory;
	sceneFactory = std::make_unique<SceneFactory>();
	sceneManager->Initialize(sceneFactory, "GameScene");
}

void MyGame::Update()
{
	// 更新処理
	Framework::Update();
	collisionManager->CheckCollisions();
}

void MyGame::Draw()
{
	// 描画処理
	dxCommon->PreDraw();
	spMan->PreDraw();
	sceneManager->Draw();
	imGuiManager->Draw();
	dxCommon->PostDraw();
}

void MyGame::Finalize()
{
	// 終了処理
	Framework::Finalize();
}