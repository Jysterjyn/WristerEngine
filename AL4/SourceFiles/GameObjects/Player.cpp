#include "Player.h"
#include <imgui.h>
#include <ImGuiManager.h>

void Player::Initialize()
{
	WristerEngine::_3D::ModelManager* modelManager = WristerEngine::_3D::ModelManager::GetInstance();
	objects = modelManager->CreateGroup("Player", true);
	rootPos.translation.y = 2;
	rootPos.scale *= 0.4f;
	rootPos.rotation.y = 0;

	for (auto o : objects)
	{
		o.second->material.ambient = { 0,0,0 };
		o.second->transform.parent = &rootPos;
	}

	objects["chest"]->transform.translation.y = 1.0f;
	objects["head"]->transform.translation.y = 3.5f;
	objects["handLeft"]->transform.translation = { 1.4f,1.7f,0 };
	objects["handRight"]->transform.translation = { -1.4f,1.7f,0 };
	//objects["footLeft"]->transform.translation = { -0.6f,-0.3f,0 };
	//objects["footRight"]->transform.translation = { 0.6f,-0.3f,0 };

	const float DEAD_ZONE = 0.7f;
	if (input->IsConnectGamePad()) { input->SetDeadZone(0, DEAD_ZONE, DEAD_ZONE); }
}

void Player::Move()
{
	if (!input->IsConnectGamePad()) { return; }
	bool isMoving = false;

	// 速さ
	const float SPEED = 0.3f;

	// 移動量
	Vector2 padMove = input->ConLStick(0, SPEED);
	Vector3 move = { padMove.x,0.0f,padMove.y };

	isMoving = move.Length() != 0;

	// 移動ベクトルをカメラの角度だけ回転する
	Matrix4 rotMat = Matrix4::RotateY(camera->GetTransform()->rotation.y);
	move *= rotMat;

	if (isMoving)
	{
		// 移動
		rootPos.translation += move;
		// 目標角度の算出
		goalAngle = std::atan2(move.x, move.z);
	}

	rootPos.rotation.y = LerpShortAngle(rootPos.rotation.y, goalAngle, 0.4f);
	rootPos.Update();
}

void Player::InitializeFloatingGimmick()
{
	floatingParameter = 0.0f;
}

void Player::UpdateFloatingGimmick()
{
	const uint16_t CYCLE = 60;
	const float STEP = 2.0f * PI / CYCLE;

	floatingParameter += STEP;
	floatingParameter = std::fmod(floatingParameter, 2.0f * PI);
}

void Player::Update()
{
	Move();
}

void Player::Draw()
{
}
