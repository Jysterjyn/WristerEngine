#include "PlayerBehavior.h"

std::unordered_map<std::string, WE::_3D::Object3d*>* BaseBehavior::objects = nullptr;
WE::_3D::Transform* BaseBehavior::rootPos = nullptr;
const WE::_3D::BaseCamera* BaseBehavior::camera = nullptr;
std::optional<BaseBehavior::Behavior> BaseBehavior::behaviorRequest = std::nullopt;
float BaseBehavior::destinationAngleY = 0.0f;
Vector3 BaseBehavior::velocity;
WE::GlobalVariables* BaseBehavior::globalVariables = WE::GlobalVariables::GetInstance();
WE::Input* BaseBehavior::input = WE::Input::GetInstance();

const std::array<AttackBehavior::ConstAttack, AttackBehavior::ComboNum> AttackBehavior::kConstAttacks =
{
	{
		{0, 0, 20, 0, 0.0f, 0.0f, 0.15f},
		{15, 10, 15, 0, 0.2f, 0.0f, 0.0f},
		{15, 10, 15, 30, 0.2f, 0.0f, 0.0f},
	}
};


void BaseBehavior::Move()
{
	if (!input->IsConnectGamePad()) { return; }
	bool isMoving = false;

	// ‘¬‚³
	const float SPEED = 0.3f;

	// ˆÚ“®—Ê
	Vector2 padMove = input->ConLStick(0, SPEED);
	velocity = { padMove.x,0.0f,padMove.y };

	isMoving = velocity.Length() != 0;

	// ˆÚ“®ƒxƒNƒgƒ‹‚ðƒJƒƒ‰‚ÌŠp“x‚¾‚¯‰ñ“]‚·‚é
	Matrix4 rotMat = Matrix4::RotateY(camera->GetTransform()->rotation.y);
	velocity *= rotMat;

	if (isMoving)
	{
		// ˆÚ“®
		rootPos->translation += velocity;
		// –Ú•WŠp“x‚ÌŽZo
		destinationAngleY = std::atan2(velocity.x, velocity.z);
	}

	rootPos->rotation.y = LerpShortAngle(rootPos->rotation.y, destinationAngleY, 0.4f);
}

void RootBehavior::Initialize()
{
	ApplyGlobalVariables();
	(*objects)["sword"]->isInvisible = true;

	globalVariables->AddItem(groupName, "floatCycle", cycle);
	globalVariables->AddItem(groupName, "amplitude", amplitude);
	globalVariables->AddItem(groupName, "idleArmAngleMax", idleArmAngleMax);
}

void RootBehavior::Update()
{
	ApplyGlobalVariables();
	Move();

	const float STEP = 2.0f * PI / cycle;

	parameter += STEP;
	parameter = std::fmod(parameter, 2.0f * PI);
	(*objects)["chest"]->transform.translation.y = std::sin(parameter) * amplitude;
	float param = std::sin(parameter) * idleArmAngleMax;
	(*objects)["handLeft"]->transform.rotation.x = param;
	(*objects)["handRight"]->transform.rotation.x = -param;

	if (!input->IsConnectGamePad()) { return; }
	if (input->IsTrigger(0, WE::JoyPad::A)) { behaviorRequest = Behavior::Attack; }
	if (input->IsTrigger(0, WE::JoyPad::Y)) { behaviorRequest = Behavior::Dash; }
	if (input->IsTrigger(0, WE::JoyPad::B)) { behaviorRequest = Behavior::Jump; }
}

void RootBehavior::ApplyGlobalVariables()
{
	cycle = globalVariables->GetValue<int32_t>(groupName, "floatCycle");
	amplitude = globalVariables->GetValue<float>(groupName, "amplitude");
	idleArmAngleMax = globalVariables->GetValue<float>(groupName, "idleArmAngleMax");
}

void AttackBehavior::Initialize()
{
	ApplyGlobalVariables();
	globalVariables->AddItem(groupName, "attackCycle", 20);
	(*objects)["sword"]->isInvisible = false;
	globalVariables->AddItem(groupName, "Sword Translation", (*objects)["sword"]->transform.translation);
}

void AttackBehavior::Update()
{
	ApplyGlobalVariables();
	
	//if (workAttack.comboIndex >= ComboNum - 1)
	//{
	//	if (input->IsTrigger(0, WE::JoyPad::A)) { workAttack.comboNext = true; }
	//}

	//if (++workAttack.attackParameter >= kConstAttacks[workAttack.comboIndex].GetComboTime())
	//{
	//	if (workAttack.comboNext)
	//	{
	//		workAttack.comboIndex++;
	//		BehaviorAttackInitialize();
	//		Move();
	//	}
	//	else
	//	{
	//		behaviorRequest = Behavior::Root;
	//	}
	//}

	//switch (workAttack.comboIndex)
	//{
	//case 0:
	//	break;
	//case 1:
	//	break;
	//case 2:
	//default:
	//	break;
	//}

	const float STEP = Angle(90) / cycle;
	const float END_ANGLE = Angle(90);

	parameter += STEP;
	// UŒ‚I—¹
	if (parameter >= END_ANGLE) { behaviorRequest = Behavior::Root; }
	parameter = min(parameter, END_ANGLE);
	(*objects)["handLeft"]->transform.rotation.x = -Angle(180) + parameter;
	(*objects)["handRight"]->transform.rotation.x = -Angle(180) + parameter;
	(*objects)["sword"]->transform.rotation.x = parameter;
}

void AttackBehavior::ApplyGlobalVariables()
{
	(*objects)["sword"]->transform.translation = globalVariables->GetValue<Vector3>(groupName, "Sword Translation");
	cycle = globalVariables->GetValue<int32_t>(groupName, "attackCycle");
}

void DashBehavior::Initialize()
{
	rootPos->rotation.y = destinationAngleY;
}

void DashBehavior::Update()
{
	const float SPEED = 1.5f;
	const uint32_t behaviorDashTime = 15;

	velocity = { 0.0f,0.0f,1.0f };

	Matrix4 rotMat = Matrix4::RotateY(rootPos->rotation.y);
	velocity *= rotMat;

	rootPos->translation += velocity * SPEED;

	if (++parameter >= behaviorDashTime)
	{
		behaviorRequest = Behavior::Root;
	}
}

void JumpBehavior::Initialize()
{
	rootPos->translation.y = 0;
	(*objects)["handLeft"]->transform.rotation.x = 0;
	(*objects)["handRight"]->transform.rotation.x = 0;

	const float kJumpFirstSpeed = 1.0f;
	velocity.y = kJumpFirstSpeed;
}

void JumpBehavior::Update()
{
	rootPos->translation += velocity;
	const float kGravityAcceleration = 0.05f;
	Vector3 accelerationVector = { 0,-kGravityAcceleration,0 };
	velocity += accelerationVector;

	if (rootPos->translation.y <= 0.0f)
	{
		rootPos->translation.y = 0;
		behaviorRequest = Behavior::Root;
	}
}
