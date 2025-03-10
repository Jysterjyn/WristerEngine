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
	debugCamera.Initialize({}, 10);

	WristerEngine::_3D::ModelManager::SetCamera(&debugCamera);
	player = std::make_unique<Player>();
	player->Initialize();
}

void TestScene::Update()
{
	player->Update();
	debugCamera.Update();
}
