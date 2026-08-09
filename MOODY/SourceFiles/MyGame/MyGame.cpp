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

	postEffect = WE::_2D::PostEffect::Create();
}

void MyGame::Update()
{
	// 更新処理
	Framework::Update();
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

	ImGui::Text("FPS: %f", fps->GetFPS());
	imGuiManager->PrintVector("Window Size", wAPI->GetScreenCursorPos());

	RECT rc;
	GetClientRect(wAPI->GetHwnd(), &rc);

	ImGui::Text(
		("Client = " +
			std::to_string(rc.right - rc.left) +
			" x " +
			std::to_string(rc.bottom - rc.top)).c_str());

	ImGui::Text("WIN_SIZE : %.0f x %.0f\n",
		WE::WIN_SIZE.x, WE::WIN_SIZE.y);

	ImGuiIO& io = ImGui::GetIO();

	ImGui::Text(
		"ImGui DisplaySize = %.0f x %.0f\n",
		io.DisplaySize.x,
		io.DisplaySize.y
	);
}

void MyGame::Draw()
{
	// ポストエフェクト
	postEffect->PreDrawScene();
	spMan->PreDraw();
	sceneManager->Draw();
	postEffect->PostDrawScene();

	// 描画処理
	dxCommon->PreDraw();
	postEffect->Draw();
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
