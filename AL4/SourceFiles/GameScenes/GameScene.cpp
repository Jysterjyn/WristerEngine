#include "GameScene.h"
#include "imgui.h"
#include <FollowCamera.h>
#include <DebugCamera.h>
#include <SceneManager.h>

void GameScene::Initialize()
{
	player = std::make_unique<Player>();
	player->Initialize("Player");

	const std::vector<Vector3> enemiesPos =
	{
		{0,0,0},{10,0,0},{0,0,10}
	};

	for (auto& p : enemiesPos)
	{
		std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
		newEnemy->Initialize("Tomorou");
		newEnemy->SetPosition(p);
		enemies.push_back(std::move(newEnemy));
	}

	WE::_3D::FollowCamera::Prop prop;
	prop.target = player->GetTransform();
	prop.posOffset = { 0.0f,2.0f,-12.0f };
	WE::_3D::DebugCamera::Prop dprop;
	dprop.targetPos = { 0,2,0 };
	dprop.distance = 10;
	dprop.mouseMoveDec = 200;
	dprop.wheelSpdDec = 100;
	cMan->Create("Debug", WE::_3D::CameraType::Debug, &dprop);
	cMan->Create("Player", WE::_3D::CameraType::Follow, &prop);
	BaseBehavior::SetCamera();

	skydome.Initialize("skydome", 2);
	ground.Initialize("cube", { 100,1,100 });

	const float DEAD_ZONE = 0.7f;
	input->SetDeadZone(0, DEAD_ZONE, DEAD_ZONE);

	lockOn = std::make_unique<LockOn>();
	lockOn->Initialize();

	WE::_3D::BaseCamera* camera = cMan->Get();
	WE::_3D::FollowCamera* fCamera = dynamic_cast<WE::_3D::FollowCamera*>(camera);
	if (fCamera) { fCamera->SetLockOn(lockOn.get()); }
	
	BaseBehavior::SetLockOn(lockOn.get());
}

void GameScene::Update()
{
	if (input->IsTrigger(WE::Key::_1)) { cMan->Change("Debug"); }
	if (input->IsTrigger(WE::Key::_2)) { cMan->Change("Player"); }
	if (input->IsTrigger(WE::Key::Q)) { sceneManager->ChangeScene("GameScene", true, true, false); }

	player->Update();
	for (auto& e : enemies) { e->Update(); }
	lockOn->Update(enemies);
}
