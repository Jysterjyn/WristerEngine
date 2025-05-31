#include "Player.h"
#include <imgui.h>
#include <ImGuiManager.h>
#include <GlobalVariables.h>

void Player::Initialize(const std::string& modelGroupName)
{
	BaseCharacter::Initialize(modelGroupName);

	rootPos.translation.y = 2.0f;

	for (auto& o : objects)
	{
		o.second->material.ambient = { 0,0,0 };
		o.second->transform.parent = &rootPos;
		if (o.first != "chest")
		{
			if (o.first != "sword") { o.second->transform.parent = &objects["chest"]->transform; }
			else { o.second->transform.parent = &rootPos; }
		}
		else
		{
			o.second->transform.parent = &rootPos;
			o.second->transform.scale *= 0.4f;
		}
	}

	objects["head"]->transform.translation.y = 2.5f;
	objects["handLeft"]->transform.translation = { 1.4f,0.7f,0 };
	objects["handRight"]->transform.translation = { -1.4f,0.7f,0 };
	objects["sword"]->isInvisible = true;
	//objects["footLeft"]->transform.translation = { -0.6f,-0.3f,0 };
	//objects["footRight"]->transform.translation = { 0.6f,-0.3f,0 };

	const float DEAD_ZONE = 0.7f;
	if (input->IsConnectGamePad()) { input->SetDeadZone(0, DEAD_ZONE, DEAD_ZONE); }

	InitializeFloatingGimmick();

	WE::GlobalVariables* globalVariables = WE::GlobalVariables::GetInstance();
	std::string groupName = "Player";
	globalVariables->SetValue(groupName, "Test_int32_t", 90);
	globalVariables->SetValue(groupName, "Test_float", 90.0f);
	globalVariables->SetValue(groupName, "Test_Vector3", {1,2,3});
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
		destinationAngleY = std::atan2(move.x, move.z);
	}

	rootPos.rotation.y = LerpShortAngle(rootPos.rotation.y, destinationAngleY, 0.4f);
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
	float param = std::sin(floatingParameter) * amplitude;
	objects["chest"]->transform.translation.y = param;
	objects["handLeft"]->transform.rotation.x = param;
	objects["handRight"]->transform.rotation.x = -param;

	//ImGui::Begin("Player");
	//float min = -5.0f, max = 5.0f;
	//WE::ImGuiManager::SliderVector("Head Translation", objects["head"]->transform.translation, min, max);
	//WE::ImGuiManager::SliderVector("ArmL Translation", objects["handLeft"]->transform.translation, min, max);
	//WE::ImGuiManager::SliderVector("ArmR Translation", objects["handRight"]->transform.translation, min, max);
	//WE::ImGuiManager::SliderVector("Sword Translation", objects["sword"]->transform.translation, min, max);
	//ImGui::SliderInt("cycle", &cycle, 1, 120);
	//ImGui::SliderFloat("amplitude", &amplitude, 0.0f, 5.0f);
	//ImGui::End();
}

void Player::BehaviorRootInitialize()
{
	InitializeFloatingGimmick();
	cycle = 60;
	objects["sword"]->isInvisible = true;
}

void Player::BehaviorRootUpdate()
{
	Move();
	UpdateFloatingGimmick();
	// 攻撃開始
	if (!input->IsConnectGamePad()) { return; }
	if (input->IsTrigger(0, WE::JoyPad::A)) { behaviorRequest = Behavior::Attack; }
	if (input->IsTrigger(0, WE::JoyPad::B)) { behaviorRequest = Behavior::Dash; }
}

void Player::BehaviorAttackInitialize()
{
	InitializeFloatingGimmick();
	cycle = 20;
	objects["sword"]->isInvisible = false;
}

void Player::BehaviorAttackUpdate()
{
	const float STEP = Angle(90) / cycle;
	const float END_ANGLE = Angle(90);

	floatingParameter += STEP;
	// 攻撃終了
	if (floatingParameter >= END_ANGLE) { behaviorRequest = Behavior::Root; }
	floatingParameter = min(floatingParameter, END_ANGLE);
	objects["handLeft"]->transform.rotation.x = -Angle(180) + floatingParameter;
	objects["handRight"]->transform.rotation.x = -Angle(180) + floatingParameter;
	objects["sword"]->transform.rotation.x = floatingParameter;

	//ImGui::Begin("Player");
	//float min = -2 * PI, max = 2 * PI;
	//WE::ImGuiManager::SliderVector("Head rotation", objects["head"]->transform.rotation, min, max);
	//WE::ImGuiManager::SliderVector("ArmL rotation", objects["handLeft"]->transform.rotation, min, max);
	//WE::ImGuiManager::SliderVector("ArmR rotation", objects["handRight"]->transform.rotation, min, max);
	//WE::ImGuiManager::SliderVector("Sword rotation", objects["sword"]->transform.rotation, min, max);
	//ImGui::SliderInt("cycle", &cycle, 1, 120);
	//ImGui::SliderFloat("amplitude", &amplitude, 0.0f, 5.0f);
	//ImGui::End();
}

void Player::BehaviorDashInitialize()
{
	workDash.dashParameter = 0;
	rootPos.rotation.y = destinationAngleY;
}

void Player::BehaviorDashUpdate()
{
	const float SPEED = 1.5f;
	const uint32_t behaviorDashTime = 15;

	Vector3 move = { 0.0f,0.0f,1.0f };

	Matrix4 rotMat = Matrix4::RotateY(rootPos.rotation.y);
	move *= rotMat;

	rootPos.translation += move * SPEED;

	if (++workDash.dashParameter >= behaviorDashTime)
	{
		behaviorRequest = Behavior::Root;
	}
}

void Player::Update()
{
	if (behaviorRequest)
	{
		// 振る舞いを変更する
		behavior = behaviorRequest.value();
		// 角振る舞いごとの初期化を実行
		switch (behavior)
		{
		case Behavior::Root:
		default:
			BehaviorRootInitialize();
			break;
		case Behavior::Attack:
			BehaviorAttackInitialize();
			break;
		case Behavior::Dash:
			BehaviorDashInitialize();
			break;
		}
		// 振る舞いリクエストをリセット
		behaviorRequest = std::nullopt;
	}

	switch (behavior)
	{
	case Behavior::Root:
	default:
		BehaviorRootUpdate();
		break;
	case Behavior::Attack:
		BehaviorAttackUpdate();
		break;
	case Behavior::Dash:
		BehaviorDashUpdate();
		break;
	}

	BaseCharacter::Update();
}