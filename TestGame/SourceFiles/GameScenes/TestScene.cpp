#include "TestScene.h"
#include <DebugCamera.h>

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
	//		std::unique_ptr<Ground> ground=std::make_unique<Ground>();
	//		ground->Initialize({ (j - DIV_NUM / 2) * LAND_SCALE * 2,0,
	//			(i - DIV_NUM / 2) * LAND_SCALE * 2 }, LAND_SCALE);
	//		grounds.push_back(std::move(ground));
	//	}
	//}
	//player.Initialize();

	//WE::_3D::Object3d* o1 = modelManager->Create("TestSphere", true);
	//o1->material.textures[0].color = { 1,0,0 };
	//o1->transform.translation.x = 1;

	//WE::_3D::Object3d* o2 = modelManager->Create("TestSphere");
	//o2->material.textures[0].color = { 1,0,0 };
	//o2->transform.translation.x = -1;

	modelManager->Create("cube", false);

	WE::LightGroup* lg = modelManager->GetLightGroup();
	lg->GetDirectionalLight(1)->SetActive(false);
	lg->GetDirectionalLight(2)->SetActive(false);

	WE::_3D::DebugCamera::Prop prop;
	prop.distance = 15.0f;
	prop.wheelSpdDec = 100;
	prop.mouseMoveDec = 200;
	cameraManager->Create("debug", WE::_3D::CameraType::Debug, &prop);
}

void TestScene::Update()
{
	//player.Update();
}
