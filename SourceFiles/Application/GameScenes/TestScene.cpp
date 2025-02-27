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
	sprites["bg"] = WristerEngine::_2D::Sprite::Create({ "sample.png" });
	sprites["bg2"] = WristerEngine::_2D::Sprite::Create({ "uvChecker.png" });

	timedCall = std::make_unique<WristerEngine::TimedCall>
		(std::bind(&TestSceneUIDrawer::CreateNewSprite, this), 180);
}

void TestSceneUIDrawer::Update()
{
	timedCall->Update();
}

void TestSceneUIDrawer::CreateNewSprite()
{
	sprites["bg3"] = WristerEngine::_2D::Sprite::Create({ "models/cube/cube.jpg" });
}

void TestScene::Initialize()
{
	debugCamera.Initialize();
	WristerEngine::_3D::ModelManager::SetCamera(&debugCamera);
	uiDrawer = std::make_unique<TestSceneUIDrawer>();
	uiDrawer->Initialize();
	obj = WristerEngine::_3D::ModelManager::Create("cube");
	obj->transform.scale *= 0;
	WristerEngine::ParticleManager::AddParticleGroup("kamata.ico");
}

void TestScene::Update()
{
	uiDrawer->Update();
	debugCamera.Update();

	obj->transform.translation.x +=
		input->Move(WristerEngine::Key::D, WristerEngine::Key::A, 1);

	obj->transform.translation.y +=
		input->Move(WristerEngine::Key::W, WristerEngine::Key::S, 1);

	//obj->material.SetDissolvePow(0.5f);

	//WristerEngine::ParticleGroup* pGroup = WristerEngine::ParticleManager::GetParticleGroup(0);
	//WristerEngine::DiffuseParticle::AddProp addProp;
	//addProp.posOffset.y = 10;
	//pGroup->Add(addProp);
}
