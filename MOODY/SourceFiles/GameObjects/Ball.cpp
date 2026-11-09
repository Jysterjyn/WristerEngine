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

	collider = static_cast<WE::_2D::CircleCollider*>(group->AddCollider(WE::ChangeVal(WE::_2D::CollisionShapeType::Circle)));
	if (!collider) { assert(0); return; }
	//BaseCollider::Initialize("Ball", info);
	//collider = AddCollider<WE::_2D::CircleCollider>(info);
	collider->SetTransform(sprite);
	collider->SetRadius(Half(sprite->size.x));
}

void Ball::Update()
{
	behaviorPattern.Update();
	ApplyParameter();
}

void Ball::Draw()
{
	sprite->Draw();
}

void Ball::ApplyParameter()
{
	const BehaviorParameter* pParam = GetParameter();
	if (!pParam) { return; }
	sprite->position = pParam->pos;
	sprite->color = pParam->color;
	if (pParam->isDestroy) { Destroy(); }
}

void Ball::ChangeNextBehavior(WE::CR<Vector2> pos)
{
	BehaviorParameter* pParam = GetParameter();
	if (pParam == nullptr) { return; }

	pParam->pos.x = pos.x;
}

Ball::~Ball()
{
	collider->Destroy();
}

BehaviorParameter* Ball::GetParameter()
{
	return static_cast<BehaviorParameter*>(behaviorPattern.GetParam());
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
	if (pParam->pos.x < 300) { pParam->isDestroy = true; }
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
	const float MOVE_SPD = 6.0f;
	//pParam->pos.x -= MOVE_SPD;
}

void BallManager::Initialize()
{
	WE::ColliderInfo info(WE::ChangeVal(CollisionAttribute::Ball), WE::ChangeVal(CollisionMask::Ball));
	Ball::SetColliderGroup(Collider::Initialize("Ball", info));
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
	WE::uPtr<WE::TimedCall> timedCall = std::make_unique<WE::TimedCall>([&]() { CreateBall(); }, 500);
	timedCalls.Add(std::move(timedCall));
}

void BallManager::OnCollision()
{
	if (GetCollisionPairs().empty()) { return; }

	for (auto& pair : GetCollisionPairs())
	{
		Ball* ball = (Ball*)pair.my;
		if (!ball) { continue; }

		if (pair.other->GetName() == "Player" && ball->IsUkenagashi())
		{
			// 相手がプレイヤーだったら消滅
			pair.my->Destroy();
		}
		if (pair.other->GetName() == "JudgeCircle" && WE::Input::GetInstance()->IsTrigger(WE::Key::Left))
		{
			// 相手が判定円だったら左矢印入力で受け流しモードに移行
			const WE::_2D::SingleCollider* judgeCircle = static_cast<WE::_2D::SingleCollider*>(pair.other);
			const WE::_2D::CircleCollider* ballCollider = static_cast<WE::_2D::CircleCollider*>(pair.my);
			if (!judgeCircle || !ballCollider) { return; }

			const WE::_2D::Transform* judgeTransform = judgeCircle->GetTransform();
			if (!judgeTransform) { return; }

			ball->ChangeNextBehavior(judgeTransform->position.x - judgeTransform->size.x / 4 - ballCollider->GetRadius());
		}
	}
}