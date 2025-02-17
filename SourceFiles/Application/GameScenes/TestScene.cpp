#include "TestScene.h"
#include "ModelManager.h"
#include "imgui.h"

void TestSceneUIDrawer::Initialize()
{
	sprites["bg"] = WristerEngine::_2D::Sprite::Create("sample.png");
	sprites["bg2"] = WristerEngine::_2D::Sprite::Create("uvChecker.png");
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
}

void TestScene::Update()
{
	ImGui::Text("IwataShine");
	debugCamera.Update();
}
