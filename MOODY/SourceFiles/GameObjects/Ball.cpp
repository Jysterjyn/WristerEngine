#include "Ball.h"
#include <Random.h>
#include <Input.h>
#include <CollisionInfo.h>
#include <imgui.h>

WE::ColliderGroup* Ball::group = nullptr;

void Ball::Initialize()
{
	if (!group) { assert(0); return; }

	WE::_2D::SpriteProp prop;
	prop.AddFileNames({ "Nanika/Nanika.png", "Nanika/Ukenagashi.png" });
	sprite = spMan->Create(prop);
	sprite->SetTextureIndex(0);
	sprite->SetCenterAnchor();

	CreateBehavior<Move>();
	CreateBehavior<Ukenagashi>();

	// 座標をランダムで生成
	BehaviorParameter* pParam = GetParameter();
	const float EDGE = 100; // 画面端から離す座標値
	WE::Random_Float randPosY(sprite->size.y, WE::WIN_SIZE.y - sprite->size.y);
	pParam->pos = { WE::WIN_SIZE.x + EDGE,randPosY() };

	collider = static_cast<WE::_2D::CircleCollider*>(
		group->AddCollider(WE::ChangeVal(WE::_2D::CollisionShapeType::Circle), std::nullopt, this));
	if (!collider) { assert(0); return; }
	collider->SetTransform(sprite);
	collider->SetRadius(Half(sprite->size.x));
}

void Ball::Update()
{
	behaviorPattern.Update();
	ApplyParameter();
}

void Ball::ApplyParameter()
{
	const BehaviorParameter* pParam = GetParameter();
	if (!pParam) { return; }
	sprite->position = pParam->pos;
	sprite->color = pParam->color;
	sprite->rotation = pParam->rotation;
	if (pParam->IsDestroy()) { Destroy(); }
}

bool Ball::IsUkenagashi()
{
	BehaviorParameter* pParam = GetParameter();
	return pParam ? pParam->isUkenagashi : false;
}

void Ball::ChangeNextBehavior(WE::CR<Vector2> pos)
{
	BehaviorParameter* pParam = GetParameter();
	if (pParam == nullptr) { return; }

	pParam->pos.x = pos.x;
	pParam->isUkenagashi = true;
	sprite->SetTextureIndex(1);
}

void Ball::OnQueryHit(WE::CollisionPair* info)
{
	if (IsUkenagashi()) { return; }
	// 相手がプレイヤーだったら消滅
	if (info->other->GetName() == "Player")
	{
		GetParameter()->Destroy();
		return;
	}
	// 相手が判定円だったら左矢印入力で受け流しモードに移行
	if (info->other->GetName() == "JudgeCircle" && WE::Input::GetInstance()->IsTrigger(WE::Key::Left))
	{
		const WE::_2D::SingleCollider* judgeCircle = static_cast<WE::_2D::SingleCollider*>(info->other);
		const WE::_2D::CircleCollider* ballCollider = static_cast<WE::_2D::CircleCollider*>(info->my);
		if (!judgeCircle || !ballCollider) { return; }

		const WE::_2D::Transform* judgeTransform = judgeCircle->GetTransform();

		if (ballCollider->GetTransform()->position.x < judgeTransform->position.x) { return; }

		ChangeNextBehavior(judgeTransform->position.x - judgeTransform->size.x / 4 - ballCollider->GetRadius());
	}
}

void Move::Initialize()
{
	param = std::make_unique<BehaviorParameter>();
	pParam = static_cast<BehaviorParameter*>(param.get());

	if (!pParam) { return; }
	pParam->name = "Move";
	pParam->color = { 1,0.2f,0.2f };
}

void Move::Update()
{
	const ScreenSpeed MOVE_SPD(5.0f);
	pParam->pos.x -= MOVE_SPD;
	if (pParam->pos.x < 300) { pParam->Destroy(); }
	if (pParam->isUkenagashi) { Finish(); }
}

void Ukenagashi::Initialize()
{
	pParam = static_cast<BehaviorParameter*>(param.get());

	if (!pParam) { return; }
	pParam->name = "Ukenagashi";
	pParam->color = { 0.2f,1,0.2f };
}

void Ukenagashi::Update()
{
	const ScreenSpeed MOVE_SPD(10.0f);
	pParam->pos.x -= MOVE_SPD;
	pParam->rotation += Angle(ScreenSpeed(0.1f));
	if (pParam->pos.x < 0) { pParam->Destroy(); }
}

void BallManager::Initialize()
{
	WE::ColliderInfo info(WE::ChangeVal(CollisionAttribute::Ball), WE::ChangeVal(CollisionMask::Ball), "Ball");
	Ball::SetColliderGroup(Collider::Initialize(info));
	CreateBall();
}

void BallManager::Update()
{
	timedCalls.Update();
	balls.Update();
	ImGui::Text("Ball Count: %d", balls.size());
	ImGui::Text("Collider Count: %d", group->GetColliders()->size());
}

void BallManager::CreateBall()
{
	WE::uPtr<Ball> newBall = std::make_unique<Ball>();
	balls.Add(std::move(newBall));
	WE::uPtr<WE::TimedCall> timedCall = std::make_unique<WE::TimedCall>([&]() { CreateBall(); }, 0.1f);
	timedCalls.Add(std::move(timedCall));
}

void BallManager::OnCollision()
{
	for (auto& pair : GetCollisionPairs())
	{
		pair.OnQueryHit();
	}
}