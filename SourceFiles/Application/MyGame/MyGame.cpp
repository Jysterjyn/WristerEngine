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
	windowName = L"自作エンジン";
	Framework::Initialize();

	std::unique_ptr<WristerEngine::AbstractSceneFactory> sceneFactory;
	sceneFactory = std::make_unique<SceneFactory>();
	sceneManager->Initialize(sceneFactory,"TestScene");

	ModelManager::Initialize();
	WristerEngine::ParticleManager::Initialize();
	WristerEngine::AudioManager::Initialize();
	PostEffect::StaticInitialize();
	postEffect = PostEffect::Create(PostEffect::Type::None);
}

void MyGame::Update()
{
	Framework::Update();
	ModelManager::Update();
	WristerEngine::CollisionManager::CheckAllCollisions();
	WristerEngine::Physics::ResetCollideList();
	WristerEngine::ParticleManager::Update();
	WristerEngine::AudioManager::Update();
}

void MyGame::Draw()
{
	postEffect->PreDrawScene();
	//ModelManager::DrawObjects();
	//WristerEngine::ParticleManager::Draw();
	//Sprite::PreDraw();
	postEffect->PostDrawScene();

	dxCommon->PreDraw();
	//postEffect->Draw();
	Sprite::PreDraw();
	sceneManager->Draw();
	ModelManager::DrawObjects();
	WristerEngine::NonEffectDrawer::Draw();
	ImGuiManager::Draw();
	dxCommon->PostDraw();
}

void MyGame::Finalize()
{
	WristerEngine::AudioManager::Finalize();
	Framework::Finalize();
}