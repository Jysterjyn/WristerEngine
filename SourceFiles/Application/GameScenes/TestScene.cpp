#include "TestScene.h"
#include "ModelManager.h"
#include "imgui.h"
#include <ParticleManager.h>
#include <SceneManager.h>
#include <FPS.h>

void TestSceneUIDrawer::Initialize()
{
	//sprites["bg"] = WristerEngine::_2D::Sprite::Create("sample.png");
	//sprites["bg2"] = WristerEngine::_2D::Sprite::Create("uvChecker.png");
	Update();
}

void TestScene::Initialize()
{
	debugCamera.Initialize();
	WristerEngine::_3D::ModelManager::SetCamera(&debugCamera);
	uiDrawer = std::make_unique<TestSceneUIDrawer>();
	uiDrawer->Initialize();
	obj = WristerEngine::_3D::ModelManager::Create("cube");
	obj->worldTransform->scale *= 10;
	WristerEngine::ParticleManager::AddParticleGroup("kamata.ico");
}

void TestScene::Update()
{
	debugCamera.Update();
	WristerEngine::ParticleGroup* pGroup = WristerEngine::ParticleManager::GetParticleGroup(0);
	WristerEngine::DiffuseParticle::AddProp addProp;
	addProp.posOffset.x = 20;
	addProp.lifeTime = 60000;
	addProp.addNum = 30000;
	pGroup->Add(addProp);
	const WristerEngine::FPS* fps = WristerEngine::FPS::GetInstance();

	ImGui::Text("FPS %d",fps->GetFPS());
}
