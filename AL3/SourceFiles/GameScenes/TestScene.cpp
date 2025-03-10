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
	camera = WristerEngine::_3D::ModelManager::GetCamera();
	camera->eye.z = -20;

	player = std::make_unique<Player>();
	player->Initialize();
}

void TestScene::Update()
{
#ifdef _DEBUG
	if (input->IsTrigger(WristerEngine::Key::Return)) { isDebugCameraActive = !isDebugCameraActive; }
#endif // _DEBUG
	if (isDebugCameraActive)
	{
		debugCamera.Update();
		WristerEngine::_3D::ModelManager::SetCamera(&debugCamera);
	}
	else
	{
		WristerEngine::_3D::ModelManager::SetCamera(camera);
	}

	player->Update();
}
