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
	sprites["bg"] = WristerEngine::_2D::Sprite::Create({ "sample.png","uvChecker.png" });
	sprites["bg"]->isInvisible = true;
	Update();
}

void TestSceneUIDrawer::Update()
{
	if (WristerEngine::Input::GetInstance()->IsTrigger(WristerEngine::Key::Space))
	{
		sprites["bg"]->SetTextureIndex(1);
	}

	if (WristerEngine::Input::GetInstance()->IsUp(WristerEngine::Key::Space))
	{
		sprites["bg"]->SetTextureIndex(0);
	}

	WristerEngine::_2D::AbstractUIDrawer::Update();
}

void TestScene::Initialize()
{
	debugCamera.Initialize();
	WristerEngine::_3D::ModelManager::SetCamera(&debugCamera);
	uiDrawer = std::make_unique<TestSceneUIDrawer>();
	uiDrawer->Initialize();
	obj = WristerEngine::_3D::ModelManager::Create("cube");
	obj->transform->scale *= 5;
	WristerEngine::ParticleManager::AddParticleGroup("kamata.ico");
}

void TestScene::Update()
{
	uiDrawer->Update();
	debugCamera.Update();

	obj->transform->translation.x +=
		input->Move(WristerEngine::Key::D, WristerEngine::Key::A, 1);

	obj->transform->translation.y +=
		input->Move(WristerEngine::Key::W, WristerEngine::Key::S, 1);
	
	//obj->material.SetDissolvePow(0.5f);

	//WristerEngine::ParticleGroup* pGroup = WristerEngine::ParticleManager::GetParticleGroup(0);
	//WristerEngine::DiffuseParticle::AddProp addProp;
	//addProp.posOffset.y = 10;
	//pGroup->Add(addProp);
}
