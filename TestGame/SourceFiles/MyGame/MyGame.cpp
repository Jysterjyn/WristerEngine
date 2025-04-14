#include "AudioManager.h"
#include "CollisionManager.h"
#include "MyGame.h"
#include "ParticleManager.h"
#include "NonEffectDrawer.h"
#include "SceneFactory.h"
#include <ImGuiManager.h>
#include <FbxLoader.h>
using namespace WristerEngine::_2D;
using namespace WristerEngine::_3D;

void MyGame::Initialize()
{
	windowName = L"自作エンジン";
	Framework::Initialize();

	std::unique_ptr<WristerEngine::AbstractSceneFactory> sceneFactory;
	sceneFactory = std::make_unique<SceneFactory>();
	sceneManager->Initialize(sceneFactory, "TestScene");

	modelManager->Initialize();
	WristerEngine::ParticleManager::Initialize();

	WristerEngine::_3D::FbxLoader::Initialize();
}

void MyGame::Update()
{
	Framework::Update();
	modelManager->Update();
	WristerEngine::CollisionManager::CheckAllCollisions();
	WristerEngine::Physics::ResetCollideList();
	WristerEngine::ParticleManager::Update();
}

void MyGame::Draw()
{
	// 描画処理
	dxCommon->PreDraw();
	//Sprite::PreDraw();
	sceneManager->Draw();
	//modelManager->DrawObjects();
	//WristerEngine::ParticleManager::Draw();
	dxCommon->SetDescriptorHeap();
	ImGuiManager::Draw();
	dxCommon->PostDraw();
}

void MyGame::Finalize()
{
	Framework::Finalize();
	WristerEngine::_3D::FbxLoader::Finalize();
}