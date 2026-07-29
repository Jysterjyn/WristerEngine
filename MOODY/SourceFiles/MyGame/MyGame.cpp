#include "MyGame.h"
#include "SceneFactory.h"
#include <ImGuiManager.h>
#include <imgui.h>

void MyGame::Initialize()
{
	// フレームワーク初期化
	windowName = L"WristerEngine";
	Framework::Initialize();

	// シーンの生成
	std::unique_ptr<WE::ISceneFactory> sceneFactory;
	sceneFactory = std::make_unique<SceneFactory>();
	sceneManager->Initialize(sceneFactory, "GameScene");
}

void MyGame::Update()
{
	// 更新処理
	Framework::Update();
	collisionManager->CheckCollisions();

	ImGui::Text("FPS: %f", fps->GetFPS());
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

bool MyGame::IsEndRequest()
{
	return input->IsTrigger(WE::Key::Escape);
}
