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
	ground.Initialize();
	//sphere.Initialize();
	//triangle.Initialize();
	//ray.Initialize();
	player.Initialize();

	WE::_3D::DebugCamera::Prop prop;
	prop.distance = 15.0f;
	prop.wheelSpdDec = 100;
	prop.mouseMoveDec = 200;
	cameraManager->Create("debug", WE::_3D::CameraType::Debug, &prop);
}

void TestScene::Update()
{
	if (input->IsTrigger(WE::Key::K)) { BaseObject::ChangeIndex(); }
	//ground.Update();
	//sphere.Update();
	//triangle.Update();
	//ray.Update();
	player.Update();
}
