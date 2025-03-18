#include "TestScene.h"
#include "ModelManager.h"
#include "imgui.h"
#include "imguimanager.h"
#include <SceneManager.h>

void TestSceneUIDrawer::Initialize()
{
}

void TestSceneUIDrawer::Update()
{
}

void TestScene::Initialize()
{
	debugCamera.Initialize({}, 50);
	modelManager->AddCamera("debug", &debugCamera);
	camera = modelManager->GetCamera();
	camera->eye.z = -50;

	player = std::make_unique<Player>();
	player->Initialize();
	player->SetParent(railCamera.GetTransform());

	enemy = std::make_unique<Enemy>();
	enemy->Initialize();
	enemy->SetPlayer(player.get());

	skydome = std::make_unique<WristerEngine::Skydome>();
	skydome->Initialize("skydome", 1);

	railCamera.Initialize({ 0,0,-50 });

	primitiveDrawer->DrawLine3d({ -5,0,0 }, { 5,0,0 }, WristerEngine::ColorRGBA::Red());
	primitiveDrawer->DrawLine3d({ -5,5,0 }, { 5,5,0 }, WristerEngine::ColorRGBA::Blue());
	primitiveDrawer->TransferVertices();
}

void TestScene::Update()
{
#ifdef _DEBUG
	if (input->IsTrigger(WristerEngine::Key::Return)) { isDebugCameraActive = !isDebugCameraActive; }
#endif // _DEBUG
	if (isDebugCameraActive)
	{
		debugCamera.Update();
		modelManager->SetCameraName("debug");
	}
	else
	{
		modelManager->SetCameraName("default");
	}

	player->Update();
	if (enemy) { enemy->Update(); }
	//railCamera.Update();
}
