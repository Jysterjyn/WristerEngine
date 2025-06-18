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
	//ground.Initialize("cube", { 10,0.01f,10 });
	//sphere.Initialize();
	triangle.Initialize();
	ray.Initialize();

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
	triangle.Update();
	ray.Update();
}
