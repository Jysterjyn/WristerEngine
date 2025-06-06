#include "AudioManager.h"
#include "CollisionManager.h"
#include "MyGame.h"
#include "ParticleManager.h"
#include "NonEffectDrawer.h"
#include "SceneFactory.h"
#include <ImGuiManager.h>
#include <CameraManager.h>
using namespace WE::_2D;
using namespace WE::_3D;

void MyGame::Initialize()
{
	windowName = L"AL4";
	Framework::Initialize();

	std::unique_ptr<WE::AbstractSceneFactory> sceneFactory;
	sceneFactory = std::make_unique<SceneFactory>();
	sceneManager->Initialize(sceneFactory, "GameScene");

	modelManager->Initialize();
	WE::ParticleManager::Initialize();
	WE::ParticleManager::AddParticleGroup("particle3.png");
}

void MyGame::Update()
{
	Framework::Update();
	modelManager->Update();
	WE::CollisionManager::CheckAllCollisions();
	WE::Physics::ResetCollideList();
	WE::ParticleManager::Update();
}

void MyGame::Draw()
{
	// 描画処理
	dxCommon->PreDraw();
	modelManager->Draw();
	Sprite::PreDraw();
	sceneManager->Draw();
	WE::ParticleManager::Draw();
	WE::ImGuiManager::Draw();
	dxCommon->PostDraw();
}

void MyGame::Finalize()
{
	Framework::Finalize();
}