#include "MyGame.h"
#include "SceneFactory.h"
#include <ImGuiManager.h>

static int objDelNum = -1;

void MyGame::Initialize()
{
	// シーンの生成
	std::unique_ptr<WE::ISceneFactory> sceneFactory;
	sceneFactory = std::make_unique<SceneFactory>();
	sceneManager->Initialize(sceneFactory, "GameScene");
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

	if (input->IsAnyInput()) { objDelNum++; }
	//testList.Update();
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

void TestClass::Update()
{
	if (objNum == objDelNum) { isDelete = true; }
	ImGui::Text("Obj%d Updated!", objNum);
}
