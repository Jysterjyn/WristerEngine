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
	//Update();
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

	obj->worldTransform->translation.x +=
		input->Move(WristerEngine::Key::D, WristerEngine::Key::A, 1);

	obj->worldTransform->translation.y +=
		input->Move(WristerEngine::Key::W, WristerEngine::Key::S, 1);

	WristerEngine::ParticleGroup* pGroup = WristerEngine::ParticleManager::GetParticleGroup(0);
	WristerEngine::DiffuseParticle::AddProp addProp;
	addProp.posRange = 1;
	addProp.addNum = 10;
	addProp.posOffset.y = 20;
	addProp.parent = obj->worldTransform.get();
	pGroup->Add(addProp);
	const WristerEngine::FPS* fps = WristerEngine::FPS::GetInstance();

	ImGui::Text("FPS %d",fps->GetFPS());
}
