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

	railCamera.Initialize({0,0,-50});
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
		//modelManager->SetCameraName("default");
	}

	player->Update();
	if (enemy) { enemy->Update(); }
	railCamera.Update();
}
