#include "MyGame.h"
#include "SceneFactory.h"
#include <ImGuiManager.h>
#include <FadeManager.h>

void MyGame::Initialize()
{
	WE::InitializeSceneManagerParam param;
	param.startScene = "GameScene";
	param.sceneFactory = std::make_unique<SceneFactory>();
	param.fadeManager = std::make_unique<FadeManager>();
	// シーンの生成
	sceneManager->Initialize(param);
}

void MyGame::Update()
{
	// 更新処理
	ImGuiTexts();
	collisionManager->CheckCollisions();

	if (input->IsTrigger(WE::Key::D))
	{
		WE::DirectXCommon::GetInstance()->ChangeResolution(WE::UltraHD * 4.0f);
	}

	if (input->IsTrigger(WE::Key::F))
	{
		WE::DirectXCommon::GetInstance()->ChangeResolution(WE::UltraHD);
	}

	if (input->IsTrigger(WE::Key::G))
	{
		WE::DirectXCommon::GetInstance()->ChangeResolution(WE::WQHD);
	}

	if (input->IsTrigger(WE::Key::H))
	{
		WE::DirectXCommon::GetInstance()->ChangeResolution(WE::FullHD);
	}

	if (input->IsTrigger(WE::Key::J))
	{
		WE::DirectXCommon::GetInstance()->ChangeResolution(WE::FullHD / 4);
	}
}

void MyGame::ImGuiTexts()
{
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

bool MyGame::IsEndRequest()
{
	bool isEnd = input->IsTrigger(WE::Key::Escape);
	isEnd |= input->IsTrigger(WE::Mouse::Middle);
	return isEnd;
}
