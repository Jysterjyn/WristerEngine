#include "PlayerBehavior.h"
#include <imgui.h>
#include <Enemy.h>
#include <ParticleManager.h>

std::unordered_map<std::string, WE::_3D::Object3d*>* BaseBehavior::objects = nullptr;
WE::_3D::Transform* BaseBehavior::rootPos = nullptr;
const WE::_3D::BaseCamera* BaseBehavior::camera = nullptr;
std::optional<BaseBehavior::Behavior> BaseBehavior::behaviorRequest = std::nullopt;
float BaseBehavior::destinationAngleY = 0.0f;
Vector3 BaseBehavior::velocity;
WE::GlobalVariables* BaseBehavior::globalVariables = WE::GlobalVariables::GetInstance();
WE::Input* BaseBehavior::input = WE::Input::GetInstance();
const WE::_3D::LockOn* BaseBehavior::lockOn = nullptr;

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

	// 移動量
	Vector2 padMove = input->ConLStick(0, moveSpeed);
	isMoving = padMove.Length() != 0;

	velocity = { padMove.x,0.0f,padMove.y };
	// 移動ベクトルをカメラの角度だけ回転する
	Matrix4 rotMat = Matrix4::RotateY(camera->GetTransform()->rotation.y);
	velocity *= rotMat;

	if (isMoving)
	{
		// 移動
		rootPos->translation += velocity;
		// 目標角度の算出
		destinationAngleY = std::atan2(velocity.x, velocity.z);

		rootPos->rotation.y = LerpShortAngle(rootPos->rotation.y, destinationAngleY, 0.4f);
	}
	else if (lockOn && lockOn->ExistTarget())
	{
		Vector3 lockOnPos = lockOn->GetTargetPosition();
		Vector3 sub = lockOnPos - rootPos->translation;
		rootPos->rotation.y = std::atan2(sub.x, sub.z);
	}
}

void RootBehavior::Initialize()
{
	ApplyGlobalVariables();

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
	//(*objects)["sword"]->isInvisible = false;
	globalVariables->AddItem(groupName, "Sword Translation", (*objects)["sword"]->transform.translation);
	moveSpeed = 0.0f;
	sword.SetParent(rootPos);
}

void AttackBehavior::Update()
{
	ApplyGlobalVariables();

	if (comboIndex <= ComboNum - 1)
	{
		//if (input->IsTrigger(0, WE::JoyPad::A)) { comboNext = true; }
	}

	if (++parameter >= kConstAttacks[comboIndex].GetComboTime())
	{
		if (comboNext)
		{
			comboIndex++;
			parameter = 0;
			attackParameter = 0.0f;
			inComboPhase = 0;
		}
		else
		{
			behaviorRequest = Behavior::Root;
			//(*objects)["sword"]->isInvisible = true;
		}
	}

	switch (comboIndex)
	{
	case 0:
		Combo1();
		break;
	case 1:
		Combo2();
		break;
	case 2:
	default:
		Combo3();
		break;
	}

	if (!input->IsConnectGamePad()) { return; }

	if (lockOn && lockOn->ExistTarget())
	{
		Vector3 lockOnPos = lockOn->GetTargetPosition();
		Vector3 sub = lockOnPos - rootPos->translation;

		float distance = sub.Length();
		const float threshold = 0.2f;
		if (distance > threshold)
		{
			rootPos->rotation.y = std::atan2(sub.x, sub.z);
			if (moveSpeed > distance - threshold)
			{
				moveSpeed = distance - threshold;
			}
		}
	}

	velocity = { 0,0,moveSpeed };
	Matrix4 rotMat = Matrix4::RotateY(rootPos->rotation.y);
	velocity *= rotMat;

	// 移動
	rootPos->translation += velocity;
}

void AttackBehavior::ApplyGlobalVariables()
{
	//(*objects)["sword"]->transform.translation = globalVariables->GetValue<Vector3>(groupName, "Sword Translation");
}

void AttackBehavior::Combo1()
{
	const ConstAttack at = kConstAttacks[comboIndex];

	if (inComboPhase == 0)
	{
		if (parameter >= at.GetComboTime(0)) { inComboPhase++; }
	}
	if (inComboPhase == 1)
	{
		if (parameter >= at.GetComboTime(1)) { inComboPhase++; }
	}
	if (inComboPhase == 2)
	{
		if (parameter >= at.GetComboTime(2)) { inComboPhase++; }

		moveSpeed = at.swingSpeed;
		const float STEP = Angle(90) / at.swingTime;
		const float END_ANGLE = Angle(90);

		attackParameter += STEP;
		min(END_ANGLE, attackParameter);

		(*objects)["handLeft"]->transform.rotation.x = -Angle(180) + attackParameter;
		(*objects)["handRight"]->transform.rotation.x = -Angle(180) + attackParameter;
		sword.SetRotation({ attackParameter,0,0 });
		sword.Update();
	}
}

void AttackBehavior::Combo2()
{
}

void AttackBehavior::Combo3()
{
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

void Sword::OnCollision([[maybe_unused]] WE::ColliderGroup* colliderGroup)
{
	for (auto& pair : group->GetCollisionPair())
	{
		if (pair.first->GetAttribute() != ChangeVal(CollisionAttribute::PlayerWeapon)) { continue; }
		uint32_t typeID = pair.second->GetAttribute();
		if (typeID == ChangeVal(CollisionAttribute::Enemy))
		{
			Enemy* enemy = static_cast<Enemy*>(pair.second->GetOwner());
			uint32_t serialNumber = enemy->GetSerialNumber();

			if (contactRecord.CheckRecord(serialNumber)) { return; }
			contactRecord.AddRecord(serialNumber);

			WE::ParticleGroup* pGroup = WE::ParticleManager::GetParticleGroup(0);
			WE::DiffuseParticle::AddProp addProp;
			addProp.posOffset = enemy->GetCenterPos();
			addProp.posRange = {};
			addProp.velRange = {};
			addProp.accRange = {};
			addProp.startScale = 1.0f;
			addProp.endScale = 3.0f;
			pGroup->Add(addProp);
		}
	}
}
