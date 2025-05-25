#include "Player.h"
#include <imgui.h>
#include <ImGuiManager.h>

void Player::Initialize()
{
	WristerEngine::_3D::ModelManager* modelManager = WristerEngine::_3D::ModelManager::GetInstance();
	objects = modelManager->CreateGroup("Player", true);
	rootPos.translation.y = 2;
	rootPos.scale *= 0.4f;
	rootPos.rotation.y = Angle(180);

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
}

void Player::Move()
{
	if (!input->IsConnectGamePad()) { return; }
	// 速さ
	const float SPEED = 0.3f;

	// 移動量
	Vector2 padMove = input->ConLStick(0, SPEED);
	Vector3 move = { padMove.x,0.0f,padMove.y };

	// 移動ベクトルをカメラの角度だけ回転する
	Matrix4 rotMat = Matrix4::RotateY(camera->GetTransform()->rotation.y);
	move *= rotMat;

	if (move.Length() != 0) { rootPos.rotation.y = std::atan2(move.x, move.z) + Angle(180); }

	// 移動
	rootPos.translation += move;
	rootPos.Update();
}

void Player::Update()
{
	Move();

	ImGui::Text("IsConnectGamePad = %d", input->IsConnectGamePad());
	WE::Input::PadState padState = input->GetPadState(0);
	ImGui::Text("padState.lt_rt = %d", padState.lt_rt);
	ImGui::Text("padState.lX = %d", padState.lX);
	ImGui::Text("padState.lY = %d", padState.lY);
	ImGui::Text("padState.rX = %d", padState.rX);
	ImGui::Text("padState.rY = %d", padState.rY);
	WE::ImGuiManager::PrintVector("padState.dirKey", padState.dirKey);
	DIJOYSTATE joyState = input->GetJoyState(0);
	for (size_t i = 0; i < _countof(joyState.rgbButtons); i++)
	{
		ImGui::Text("joyState.rgbButtons[%d] = %d", i, joyState.rgbButtons[i]);
	}
}

void Player::Draw()
{
}
