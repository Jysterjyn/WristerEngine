#include "AudioManager.h"
#include "CollisionManager.h"
#include "MyGame.h"
#include "NonEffectDrawer.h"
#include "SceneFactory.h"
#include <ImGuiManager.h>
using namespace WE::_2D;
using namespace WE::_3D;

void MyGame::Initialize()
{
	windowName = L"自作エンジン";
	Framework::Initialize();

	std::unique_ptr<WE::AbstractSceneFactory> sceneFactory;
	sceneFactory = std::make_unique<SceneFactory>();
	sceneManager->Initialize(sceneFactory, "TestScene");

	modelManager->Initialize();
	particleManager->Initialize();
	particleManager->AddParticleGroup("testparticle.png", WE::ParticleType::Dark);

	pDrawer->Initialize();
}

void MyGame::Update()
{
	Framework::Update();
	modelManager->Update();
	collisionManager->CheckCollisions();
	modelManager->Update();
	particleManager->Update();
	pDrawer->Update();
}

void MyGame::Draw()
{
	// 描画処理
	dxCommon->PreDraw();
	sceneManager->Draw();
	modelManager->Draw();
	pDrawer->Draw();
	particleManager->Draw();
	WE::ImGuiManager::Draw();
	dxCommon->PostDraw();
}

void MyGame::Finalize()
{
	Framework::Finalize();
}