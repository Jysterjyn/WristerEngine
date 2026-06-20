#include "Ball.h"
#include <Random.h>
#include <Input.h>
#include <CollisionInfo.h>

void Ball::Initialize()
{
	sprite = spMan->Create({ "Nanika/Nanika.png","Nanika/Ukenagashi.png" });
	sprite->SetTextureIndex(0);
	sprite->SetCenterAnchor();

	CreateBehavior<Move>();
	CreateBehavior<Ukenagashi>();

	// 座標をランダムで生成
	BehaviorParameter* pParam = GetParameter();
	const float EDGE = -150; // 画面端から離す座標値
	WE::Random_Float randPosY(sprite->size.y + EDGE, WE::WIN_SIZE.y - sprite->size.y - EDGE);
	pParam->pos = { WE::WIN_SIZE.x + EDGE,randPosY() };

	WE::ColliderInfo info(ChangeVal(CollisionAttribute::Ball), ChangeVal(CollisionMask::Ball));
	Collider::Initialize("Ball", info);
	collider = AddCollider<WE::_2D::CircleCollider>(info);
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

void Ball::OnCollision()
{
	// 相手がプレイヤーだったら
	int i = 0;
	i++;

	//collider->Destroy();
	//GetParameter()->isDestroy = true;
}

void Ball::ApplyParameter()
{
	const BehaviorParameter* pParam = GetParameter();
	if (!pParam) { return; }
	sprite->position = pParam->pos;
	sprite->color = pParam->color;
	if (pParam->isDestroy) { Destroy(); }
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
	const float MOVE_SPD = 2.0f;
	pParam->pos.x -= MOVE_SPD;
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
	pParam->pos.x -= MOVE_SPD;

	if (WE::Input::GetInstance()->IsTrigger(WE::Key::Left)) { pParam->isDestroy = true; }
}