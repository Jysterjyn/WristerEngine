#include "GamePlayScene.h"
#include "ModelManager.h"
#include "imgui.h"
#include "imguimanager.h"
#include <SceneManager.h>
#include <WristerEngine.h>
#include <BaseCamera.h>
#include <WindowsAPI.h>
#include <MathUtility.h>

void GamePlaySceneUIDrawer::Initialize()
{
}

void GamePlaySceneUIDrawer::Update()
{
}

void GamePlayScene::Initialize()
{
	debugCamera = std::make_unique<WE::_3D::DebugCamera>(Vector3(), 50.0f);
	debugCamera->Initialize();
	modelManager->AddCamera("debug", debugCamera.get());
	camera = modelManager->GetCamera();
	camera->eye.z = -50;

	railCamera = std::make_unique<WE::_3D::RailCamera>(Vector3(0, 0, -50));
	railCamera->Initialize();
	modelManager->AddCamera("rail", railCamera.get());

	player = std::make_unique<Player>();
	player->Initialize();
	player->SetParent(railCamera->GetTransform());

	enemy = std::make_unique<Enemy>();
	enemy->Initialize();
	enemy->SetPlayer(player.get());

	skydome = std::make_unique<WristerEngine::Skydome>();
	skydome->Initialize("skydome", 1);
}

void GamePlayScene::Update()
{
#ifdef _DEBUG
	if (input->IsTrigger(WristerEngine::Key::Return)) { isDebugCameraActive = !isDebugCameraActive; }
#endif // _DEBUG
	if (isDebugCameraActive)
	{
		modelManager->SetCameraName("debug");
	}
	else
	{
		modelManager->SetCameraName("rail");
	}

	player->Update();
	if (enemy) { enemy->Update(); }
}
