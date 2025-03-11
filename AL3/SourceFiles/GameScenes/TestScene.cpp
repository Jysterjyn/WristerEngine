#include "TestScene.h"
#include "ModelManager.h"
#include "imgui.h"
#include <ParticleManager.h>
#include <SceneManager.h>
#include <FPS.h>
#include <Random.h>
#include <MathUtility.h>

void TestSceneUIDrawer::Initialize()
{
}

void TestSceneUIDrawer::Update()
{
}

void TestScene::Initialize()
{
	debugCamera.Initialize({}, 20);
	modelManager->AddCamera("debug", &debugCamera);
	camera = modelManager->GetCamera();
	camera->eye.z = -20;

	player = std::make_unique<Player>();
	player->Initialize();

	enemy = std::make_unique<Enemy>();
	enemy->Initialize();
	enemy->SetPlayer(player.get());
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
}
