#include "AudioManager.h"
#include "CollisionManager.h"
#include "MyGame.h"
#include "ParticleManager.h"
#include "NonEffectDrawer.h"
#include "SceneFactory.h"
#include <ImGuiManager.h>
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
	WE::ParticleManager::Initialize();
	WE::ParticleManager::AddParticleGroup("particle1.png");
}

void MyGame::Update()
{
	Framework::Update();
	modelManager->Update();
	collisionManager->CheckCollisions();
	modelManager->Update();
	WristerEngine::ParticleManager::Update();
}

void MyGame::Draw()
{
	// 描画処理
	dxCommon->PreDraw();
	sceneManager->Draw();
	modelManager->Draw();
	WristerEngine::ParticleManager::Draw();
	WE::ImGuiManager::Draw();
	dxCommon->PostDraw();
}

void MyGame::Finalize()
{
	Framework::Finalize();
}