#include "TestScene.h"
#include "ModelManager.h"
#include "imgui.h"
#include "imguimanager.h"
#include <ParticleManager.h>
#include <SceneManager.h>
#include <FPS.h>
#include <Random.h>
#include <MathUtility.h>
#include <Quaternion.h>

void TestSceneUIDrawer::Initialize()
{
}

void TestSceneUIDrawer::Update()
{
}

void TestScene::Initialize()
{
	debugCamera.Initialize({}, 20);
	modelManager->AddCamera("debug", &debugCamera);
	camera = modelManager->GetCamera();
	camera->eye.z = -20;

	player = std::make_unique<Player>();
	player->Initialize();

	enemy = std::make_unique<Enemy>();
	enemy->Initialize();
	enemy->SetPlayer(player.get());

	for (auto& o : obj)
	{
		o = modelManager->Create("cube");
	}
	obj[0]->transform.translation = v0;
	obj[1]->transform.translation = v1;

	sphere = modelManager->Create("sphere", true);
	sphere->transform.translation = (v0 + v1) / 2.0f;
	sphere->transform.scale *= Length(v1 - v0) / 2.0f;
	sphere->material.alpha = 0.5f;
}

void TestScene::Update()
{
#ifdef _DEBUG
	if (input->IsTrigger(WristerEngine::Key::Return)) { isDebugCameraActive = !isDebugCameraActive; }
#endif // _DEBUG
	if (isDebugCameraActive)
	{
		debugCamera.Update();
		modelManager->SetCameraName("debug");
	}
	else
	{
		modelManager->SetCameraName("default");
	}

	//Vector3 v01 = Normalize(v1 - v0);
	//Vector3 av01 = (v0 + v1) / 2.0f;
	//Vector3 av0 = v0 - av01;
	//Vector3 crossZY = Normalize(Cross(v01, Vector3::MakeAxis(Axis::Y)));
	//Vector3 cross = Normalize(Cross(v01, crossZY));

	//Quaternion q0, q1, q01;
	//q0 = Quaternion::MakeAxisAngle(cross, Angle(0));
	//q1 = Quaternion::MakeAxisAngle(cross, Angle(180));
	ImGui::SliderFloat("t", &t, 0, 1);
	//q01 = Slerp(q0, q1, t);

	//obj[2]->transform.translation = Quaternion::RotateVector(av0, q01) + av01;
	obj[2]->transform.translation = Slerp(v0, v1, t);

	//WristerEngine::_2D::ImGuiManager::PrintVector("pos", obj[2]->transform.translation);
	//WristerEngine::_2D::ImGuiManager::PrintVector("cross", cross);

	//Quaternion q0, q1, q01;
	//q0 = Quaternion::MakeAxisAngle(-Vector3::MakeAxis(Axis::Z), Angle(0));
	//q1 = Quaternion::MakeAxisAngle(-Vector3::MakeAxis(Axis::Z), Angle(180));
	//ImGui::SliderFloat("t", &t, 0, 1);
	//q01 = Slerp(q0, q1, t);
	//Vector3 v = { -5,0,0 };
	//v = Quaternion::RotateVector(v, q01);
	//obj->transform.translation = v;

	player->Update();
	if (enemy) { enemy->Update(); }
}
