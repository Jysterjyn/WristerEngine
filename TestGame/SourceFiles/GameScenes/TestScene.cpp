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
	debugCamera = std::make_unique<WristerEngine::_3D::DebugCamera>(Vector3(), 10.0f);
	debugCamera->Initialize();
	modelManager->AddCamera("debug", debugCamera.get());

	modelManager->SetCameraName("debug");

	fModel = WristerEngine::_3D::FbxLoader::LoadModelFromFile("boneTest");

	WristerEngine::_3D::FbxObject3d::SetLightGroup(modelManager->GetLightGroup());
	fTrans.Initialize();
	fObject.Initialize(&fTrans,fModel);
	fObject.PlayAnimation();

	player = std::make_unique<Player>();
	player->Initialize();
}

void TestScene::Update()
{
	player->Update();
	fObject.Update();
}
