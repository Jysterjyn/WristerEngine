#include "Player.h"
#include <imgui.h>
#include <ImGuiManager.h>

void Player::Initialize(const std::string& modelGroupName)
{
	WE::Collider::Initialize("Player");
	GetGroup()->SetAttribute(static_cast<uint32_t>(CollisionAttribute::Player));

	BaseCharacter::Initialize(modelGroupName);
	collider->SetAttribute(ChangeVal(CollisionAttribute::Player));

	group->AddCollider(WE::CollisionShapeType::Plane, this)->SetAttribute(ChangeVal(CollisionAttribute::Player));

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

	rootPos.translation.z = -10;

	objects["head"]->transform.translation.y = 2.5f;
	objects["handLeft"]->transform.translation = { 1.4f,0.7f,0 };
	objects["handRight"]->transform.translation = { -1.4f,0.7f,0 };

	BaseBehavior::SetObjects(&objects);
	BaseBehavior::SetTransform(&rootPos);

	pBehavior = std::make_unique<RootBehavior>();
	pBehavior->Initialize();
}

void Player::ApplyGlobalVariables()
{
	objects["head"]->transform.translation = globalVariables->GetValue<Vector3>(groupName, "Head Translation");
	objects["handLeft"]->transform.translation = globalVariables->GetValue<Vector3>(groupName, "ArmL Translation");
	objects["handRight"]->transform.translation = globalVariables->GetValue<Vector3>(groupName, "ArmR Translation");
}

void Player::Update()
{
	if (BaseBehavior::GetBehaviorRequest())
	{
		BaseBehavior::Behavior b = BaseBehavior::GetBehaviorRequest().value();
		// 各振る舞いごとの初期化を実行
		switch (b)
		{
		case BaseBehavior::Behavior::Root:
		default:
			pBehavior = std::make_unique<RootBehavior>();
			break;
		case BaseBehavior::Behavior::Attack:
			pBehavior = std::make_unique<AttackBehavior>();
			break;
		case BaseBehavior::Behavior::Dash:
			pBehavior = std::make_unique<DashBehavior>();
			break;
		case BaseBehavior::Behavior::Jump:
			pBehavior = std::make_unique<JumpBehavior>();
			break;
		}
		pBehavior->Initialize();
		// 振る舞いリクエストをリセット
		BaseBehavior::SetBehaviorRequest(std::nullopt);
	}

	if (pBehavior) { pBehavior->Update(); }

	ApplyGlobalVariables();
	BaseCharacter::Update();
}

void Player::OnCollision([[maybe_unused]] WE::ColliderGroup* collisionGroup)
{
}
