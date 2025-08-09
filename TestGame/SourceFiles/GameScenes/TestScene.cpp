#include "TestScene.h"
#include <DebugCamera.h>
#include <ParticleManager.h>
#include <Random.h>

void TestSceneUIDrawer::Initialize()
{
	sprites["test"] = WE::_2D::Sprite::Create({ "イリウス_国旗.png" });
	sprites["test"]->size *= 0.25f;
	sprites["test"]->anchorPoint.y = 1.0f;
	sprites["test"]->position.y = 720.0f;

	WE::BehaviorPair behavior;
	behavior.first = std::make_unique<RightMove>();
	behavior.first->Initialize();
	behaviorPattern.Add(behavior);

	behavior.first = std::make_unique<UpMove>();
	behaviorPattern.Add(behavior);
}

void TestSceneUIDrawer::Update()
{
	behaviorPattern.Update();
	const BehaviorParameter* pParam = static_cast<const BehaviorParameter*>(behaviorPattern.GetParam());
	if (!pParam) { return; }
	sprites["test"]->position = pParam->pos;
}

void TestScene::Initialize()
{
	WE::_3D::DebugCamera::Prop prop;
	prop.distance = 15.0f;
	prop.wheelSpdDec = 100;
	prop.mouseMoveDec = 200;
	cameraManager->Create("debug", WE::_3D::CameraType::Debug, &prop);

	uiDrawer = std::make_unique<TestSceneUIDrawer>();
	uiDrawer->Initialize();
}

void TestScene::Update()
{
	//player.Update();
	transform.translation.x += input->Move(WE::Key::D, WE::Key::A, 0.1f);
	transform.translation.y += input->Move(WE::Key::W, WE::Key::S, 0.1f);
	transform.Update();

	uiDrawer->Update();
}

void RightMove::Initialize()
{
	param = std::make_unique<BehaviorParameter>();
	pParam = static_cast<BehaviorParameter*>(param.get());
	if (!pParam) { return; }
	pParam->name = "Move";
	pParam->pos.y = 720;
}

void RightMove::Update()
{
	if (!pParam) { return; }
	pParam->pos.x++;
	if (timer.Update()) { Finish(); }
}

void UpMove::Initialize()
{
	if (param->name == "Move") { pParam = static_cast<BehaviorParameter*>(param.get()); }
}

void UpMove::Update()
{
	if (!pParam) { return; }
	pParam->pos.y--;
	if (timer.Update()) { Finish(); }
}
