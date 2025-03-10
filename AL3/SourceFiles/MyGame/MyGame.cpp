#include "AudioManager.h"
#include "CollisionManager.h"
#include "ModelManager.h"
#include "MyGame.h"
#include "ParticleManager.h"
#include "NonEffectDrawer.h"
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
	sceneManager->Initialize(sceneFactory,"TestScene");

	ModelManager::Initialize();
	WristerEngine::ParticleManager::Initialize();
}

void MyGame::Update()
{
	Framework::Update();
	ModelManager::Update();
	WristerEngine::CollisionManager::CheckAllCollisions();
	WristerEngine::Physics::ResetCollideList();
	WristerEngine::ParticleManager::Update();
}

void MyGame::Draw()
{
	// デスクリプタヒープをセット
	dxCommon->SetDescriptorHeap();

	// 描画処理
	dxCommon->PreDraw();
	Sprite::PreDraw();
	sceneManager->Draw();
	ModelManager::DrawObjects();
	WristerEngine::ParticleManager::Draw();
	ImGuiManager::Draw();
	dxCommon->PostDraw();
}

void MyGame::Finalize()
{
	Framework::Finalize();
}