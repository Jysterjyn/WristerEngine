#include "TestScene.h"
#include "ModelManager.h"
#include "imgui.h"
#include "imguimanager.h"
#include <SceneManager.h>
#include <WristerEngine.h>
#include <BaseCamera.h>

void TestSceneUIDrawer::Initialize()
{
}

void TestSceneUIDrawer::Update()
{
}

void TestScene::Initialize()
{
	debugCamera = std::make_unique<WE::_3D::DebugCamera>(Vector3(), 50.0f);
	debugCamera->Initialize();
	modelManager->AddCamera("debug", debugCamera.get());
	camera = modelManager->GetCamera();
	camera->eye.z = -50;

	player = std::make_unique<Player>();
	player->Initialize();
	player->SetParent(railCamera.GetTransform());

	enemy = std::make_unique<Enemy>();
	enemy->Initialize();
	enemy->SetPlayer(player.get());

	skydome = std::make_unique<WristerEngine::Skydome>();
	skydome->Initialize("skydome", 1);

	railCamera.Initialize({ 0,0,-50 });

	std::vector<Vector3> controlPoints =
	{
		{0,  0,  0},
		{10, 10, 0},
		{10, 15, 0},
		{20, 15, 0},
		{20, 0,  0},
		{30, 0,  0},
	};

	const size_t segmentCount = 100;
	std::vector<Vector3> pointsDrawing;
	for (size_t i = 0; i < segmentCount + 1; i++)
	{
		float t = 1.0f / segmentCount * i;
		Vector3 pos = SplineCurve(controlPoints, t);
		pointsDrawing.push_back(pos);
	}

	for (size_t i = 0; i < pointsDrawing.size() - 1; i++)
	{
		primitiveDrawer->DrawLine3d(pointsDrawing[i], pointsDrawing[i + 1], WE::ColorRGBA::Red());
	}
	
	primitiveDrawer->TransferVertices();
}

void TestScene::Update()
{
#ifdef _DEBUG
	if (input->IsTrigger(WristerEngine::Key::Return)) { isDebugCameraActive = !isDebugCameraActive; }
#endif // _DEBUG
	if (isDebugCameraActive)
	{
		modelManager->SetCameraName("debug");
	}
	else
	{
		modelManager->SetCameraName("rail");
	}

	player->Update();
	if (enemy) { enemy->Update(); }
	//railCamera.Update();
}
