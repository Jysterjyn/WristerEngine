#include "Player.h"
#include <imgui.h>
#include <ImGuiManager.h>

void Player::Initialize(const std::string& modelGroupName)
{
	BaseCharacter::Initialize(modelGroupName);

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

	const float DEAD_ZONE = 0.7f;
	if (input->IsConnectGamePad()) { input->SetDeadZone(0, DEAD_ZONE, DEAD_ZONE); }

	InitializeFloatingGimmick();

	std::string groupName = "Player";
	globalVariables->AddItem(groupName, "Head Translation", objects["head"]->transform.translation);
	globalVariables->AddItem(groupName, "ArmL Translation", objects["handLeft"]->transform.translation);
	globalVariables->AddItem(groupName, "ArmR Translation", objects["handRight"]->transform.translation);
	globalVariables->AddItem(groupName, "Sword Translation", objects["sword"]->transform.translation);
	globalVariables->AddItem(groupName, "cycle", cycle);
	globalVariables->AddItem(groupName, "amplitude", amplitude);
}

void Player::Move()
{
	if (!input->IsConnectGamePad()) { return; }
	bool isMoving = false;

	// 速さ
	const float SPEED = 0.3f;

	// 移動量
	Vector2 padMove = input->ConLStick(0, SPEED);
	velocity = { padMove.x,0.0f,padMove.y };

	isMoving = velocity.Length() != 0;

	// 移動ベクトルをカメラの角度だけ回転する
	Matrix4 rotMat = Matrix4::RotateY(camera->GetTransform()->rotation.y);
	velocity *= rotMat;

	if (isMoving)
	{
		// 移動
		rootPos.translation += velocity;
		// 目標角度の算出
		destinationAngleY = std::atan2(velocity.x, velocity.z);
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
	if (input->IsTrigger(0, WE::JoyPad::Y)) { behaviorRequest = Behavior::Dash; }
	if (input->IsTrigger(0, WE::JoyPad::B)) { behaviorRequest = Behavior::Jump; }
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

	velocity = { 0.0f,0.0f,1.0f };

	Matrix4 rotMat = Matrix4::RotateY(rootPos.rotation.y);
	velocity *= rotMat;

	rootPos.translation += velocity * SPEED;

	if (++workDash.dashParameter >= behaviorDashTime)
	{
		behaviorRequest = Behavior::Root;
	}
}

void Player::BehaviorJumpInitialize()
{
	rootPos.translation.y = 0;
	objects["handLeft"]->transform.rotation.x = 0;
	objects["handRight"]->transform.rotation.x = 0;

	const float kJumpFirstSpeed = 1.0f;
	velocity.y = kJumpFirstSpeed;
}

void Player::BehaviorJumpUpdate()
{
	rootPos.translation += velocity;
	const float kGravityAcceleration = 0.05f;
	Vector3 accelerationVector = { 0,-kGravityAcceleration,0 };
	velocity += accelerationVector;

	if (rootPos.translation.y <= 0.0f)
	{
		rootPos.translation.y = 0;
		behaviorRequest = Behavior::Root;
	}
}

void Player::ApplyGlobalVariables()
{
	std::string groupName = "Player";
	objects["head"]->transform.translation = globalVariables->GetValue<Vector3>(groupName, "Head Translation");
	objects["handLeft"]->transform.translation = globalVariables->GetValue<Vector3>(groupName, "ArmL Translation");
	objects["handRight"]->transform.translation = globalVariables->GetValue<Vector3>(groupName, "ArmR Translation");
	objects["sword"]->transform.translation = globalVariables->GetValue<Vector3>(groupName, "Sword Translation");
	cycle = globalVariables->GetValue<int32_t>(groupName, "cycle");
	amplitude = globalVariables->GetValue<float>(groupName, "amplitude");
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
		case Behavior::Jump:
			BehaviorJumpInitialize();
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
	case Behavior::Jump:
		BehaviorJumpUpdate();
		break;
	}

	ApplyGlobalVariables();
	BaseCharacter::Update();
}