#include "CollisionManager.h"
#include "MyGame.h"
#include "SceneFactory.h"
#include <ImGuiManager.h>
using namespace WristerEngine::_2D;
using namespace WristerEngine::_3D;

void MyGame::Initialize()
{
	windowName = L"AL3";
	Framework::Initialize();

	std::unique_ptr<WristerEngine::AbstractSceneFactory> sceneFactory;
	sceneFactory = std::make_unique<SceneFactory>();
	sceneManager->Initialize(sceneFactory, "TestScene");
	
	modelManager->Initialize();
	primitiveDrawer->Initialize();
}

void MyGame::Update()
{
	Framework::Update();
	modelManager->Update();
	primitiveDrawer->Update();
	WristerEngine::CollisionManager::CheckAllCollisions();
}

void MyGame::Draw()
{
	// デスクリプタヒープをセット
	dxCommon->SetDescriptorHeap();

	// 描画処理
	dxCommon->PreDraw();
	modelManager->DrawObjects();
	Sprite::PreDraw();
	sceneManager->Draw();
	primitiveDrawer->Draw();
	ImGuiManager::Draw();
	dxCommon->PostDraw();
}

bool MyGame::IsEndRequest()
{
	// ESCキーを押したらゲームループを抜ける
	if (input->IsTrigger(WristerEngine::Key::Escape)) { return true; }
	return Framework::IsEndRequest();
}