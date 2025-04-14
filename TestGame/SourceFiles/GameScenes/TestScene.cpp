#include "TestScene.h"
#include "ModelManager.h"
#include "imgui.h"
#include <ParticleManager.h>
#include <SceneManager.h>
#include <FPS.h>
#include <Random.h>
#include <MathUtility.h>
#include <ImGuiManager.h>

static WristerEngine::ColorRGB baseColor;
static float metalness;
static float specular;
static float roughness;

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

	fModel = WristerEngine::_3D::FbxLoader::LoadModelFromFile("SpherePBRMaps");

	WristerEngine::_3D::FbxObject3d::SetLightGroup(modelManager->GetLightGroup());
	fTrans.Initialize();
	fObject.Initialize(&fTrans,fModel);
	fObject.PlayAnimation();

	//player = std::make_unique<Player>();
	//player->Initialize();
}

void TestScene::Update()
{
	WristerEngine::_2D::ImGuiManager::ColorEdit("baseColor", baseColor);
	ImGui::SliderFloat("metalness", &metalness, 0, 1);
	ImGui::SliderFloat("specular", &specular, 0, 1);
	ImGui::SliderFloat("roughness", &roughness, 0, 1);

	fModel->SetBaseColor(baseColor);
	fModel->SetMetalness(metalness);
	fModel->SetSpecular(specular);
	fModel->SetRoughness(roughness);
	fModel->TransferMaterial();

	fObject.Update();
}
