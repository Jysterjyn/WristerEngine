#include "Player.h"
#include <imgui.h>
#include <ImGuiManager.h>

void Player::Initialize()
{
	WristerEngine::_3D::ModelManager* modelManager = WristerEngine::_3D::ModelManager::GetInstance();
	objects = modelManager->CreateGroup("Player", true);
	rootPos.translation.y = 2;
	rootPos.scale *= 0.4f;

	for (auto o : objects)
	{
		o.second->material.ambient = { 0,0,0 };
		o.second->transform.parent = &rootPos;
		if (o.first != "chest") { o.second->transform.parent = &objects["chest"]->transform; }
	}

	objects["chest"]->transform.translation.y = 1.0f;
	objects["head"]->transform.translation.y = 2.5f;
	objects["handLeft"]->transform.translation = { 1.4f,0.7f,0 };
	objects["handRight"]->transform.translation = { -1.4f,0.7f,0 };
	//objects["footLeft"]->transform.translation = { -0.6f,-0.3f,0 };
	//objects["footRight"]->transform.translation = { 0.6f,-0.3f,0 };

	const float DEAD_ZONE = 0.7f;
	if (input->IsConnectGamePad()) { input->SetDeadZone(0, DEAD_ZONE, DEAD_ZONE); }

	InitializeFloatingGimmick();
}

void Player::Move()
{
	if (!input->IsConnectGamePad()) { return; }
	bool isMoving = false;

	// ‘¬‚³
	const float SPEED = 0.3f;

	// ˆÚ“®—Ê
	Vector2 padMove = input->ConLStick(0, SPEED);
	Vector3 move = { padMove.x,0.0f,padMove.y };

	isMoving = move.Length() != 0;

	// ˆÚ“®ƒxƒNƒgƒ‹‚ðƒJƒƒ‰‚ÌŠp“x‚¾‚¯‰ñ“]‚·‚é
	Matrix4 rotMat = Matrix4::RotateY(camera->GetTransform()->rotation.y);
	move *= rotMat;

	if (isMoving)
	{
		// ˆÚ“®
		rootPos.translation += move;
		// –Ú•WŠp“x‚ÌŽZo
		goalAngle = std::atan2(move.x, move.z);
	}

	rootPos.rotation.y = LerpShortAngle(rootPos.rotation.y, goalAngle, 0.4f);
}

void Player::InitializeFloatingGimmick()
{
	floatingParameter = 0.0f;
}

void Player::UpdateFloatingGimmick()
{
	const float STEP = 2.0f * PI / cycle;

	floatingParameter += STEP;
	floatingParameter = std::fmod(floatingParameter, 2.0f * PI);
	objects["chest"]->transform.translation.y = 2.0f + std::sin(floatingParameter) * amplitude;
	objects["handLeft"]->transform.rotation.x = std::sin(floatingParameter) * amplitude;
	objects["handRight"]->transform.rotation.x = -std::sin(floatingParameter) * amplitude;

	ImGui::Begin("Player");
	WE::ImGuiManager::SliderVector("Head Translation", objects["head"]->transform.translation, -5.0f, 5.0f);
	WE::ImGuiManager::SliderVector("ArmL Translation", objects["handLeft"]->transform.translation, -5.0f, 5.0f);
	WE::ImGuiManager::SliderVector("ArmR Translation", objects["handRight"]->transform.translation, -5.0f, 5.0f);
	ImGui::SliderInt("cycle", &cycle, 1, 120);
	ImGui::SliderFloat("amplitude", &amplitude, 0.0f, 5.0f);
	ImGui::End();
}

void Player::Update()
{
	Move();
	UpdateFloatingGimmick();
	rootPos.Update();
}

void Player::Draw()
{
}
