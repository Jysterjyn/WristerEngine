#include "TestScene.h"
#include <DebugCamera.h>
#include <ParticleManager.h>
#include <Random.h>

void TestSceneUIDrawer::Initialize()
{
}

void TestSceneUIDrawer::Update()
{
}

void TestScene::Initialize()
{
	//const int32_t DIV_NUM = 10;
	//const float LAND_SCALE = 3.0f;

	//for (int32_t i = 0; i < DIV_NUM; i++)
	//{
	//	for (int32_t j = 0; j < DIV_NUM; j++)
	//	{
	//		std::unique_ptr<Ground> ground = std::make_unique<Ground>();
	//		ground->Initialize({ (j - DIV_NUM / 2) * LAND_SCALE * 2,0,
	//			(i - DIV_NUM / 2) * LAND_SCALE * 2 }, LAND_SCALE);
	//		grounds.push_back(std::move(ground));
	//	}
	//}
	//player.Initialize();

	//WE::ParticleGroup* pg = WE::ParticleManager::GetParticleGroup(0);
	//WE::DiffuseParticle::AddProp pProp;
	//pProp.velRange = {};
	//pProp.velOffset = {};
	//pProp.accRange = {};
	//pProp.accOffset = {};
	//pProp.lifeTime = 100000;
	//pg->Add(pProp);

	WE::_3D::DebugCamera::Prop prop;
	prop.distance = 15.0f;
	prop.wheelSpdDec = 100;
	prop.mouseMoveDec = 200;
	cameraManager->Create("debug", WE::_3D::CameraType::Debug, &prop);
}

void TestScene::Update()
{
	//player.Update();

	WE::ParticleManager* pm = WE::ParticleManager::GetInstance();
	WE::ParticleGroup* pgd = pm->GetParticleGroup(0, WE::ParticleType::Dark);
	WE::DirectionalParticle::Prop prop;
	WE::Random_Float rand(-Angle(180), Angle(180));
	prop.radius = 2;
	prop.start.x = -5;
	prop.end.x = 5;
	prop.angle = rand();
	pgd->Add(prop);
	WE::ParticleGroup* pgl = pm->GetParticleGroup(0);
	prop.end.x = -5;
	prop.start.x = 5;
	prop.angle = rand();
	pgl->Add(prop);
}
