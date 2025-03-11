#include "Enemy.h"
#include <Imgui.h>
#include <cassert>
#include <Player.h>
using namespace WristerEngine::_3D;

void Enemy::FireAndReset()
{
	//Fire();
	//FireReserve();
}

void Enemy::Initialize()
{
	obj = ModelManager::GetInstance()->Create("enemy1");
	obj->transform.scale *= 0.5f;
	obj->transform.rotation.y = 90;
	obj->transform.translation = { 5,2,50 };

	std::unique_ptr<BaseEnemyState> nextState;
	nextState = std::make_unique<EnemyStateApproach>();
	ChangeState(std::move(nextState));
	state->Initialize();
}

void Enemy::Update()
{
	bullets.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->IsDead(); });

	timedCalls.remove_if([](std::unique_ptr<WristerEngine::TimedCall>& timedCall) {
		return timedCall->IsFinished();	});

	state->Update();
	for (auto& timedCall : timedCalls) { timedCall->Update(); }
	for (auto& bullet : bullets) { bullet->Update(); }
	
	ImGui::Text("timedCallsNum %d", timedCalls.size());
}

void Enemy::Fire()
{
	assert(player);

	const float BULLET_SPEED = 1.0f;

	Vector3 pPos = player->GetWorldPosition();
	Vector3 ePos = obj->transform.GetWorldPosition();
	Vector3 velocity = Normalize(pPos - ePos);
	velocity *= BULLET_SPEED;

	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(obj->transform.translation, velocity);
	newBullet->SetPlayer(player);
	bullets.push_back(std::move(newBullet));
}

void Enemy::FireReserve()
{
	timedCalls.push_back(std::move(std::make_unique<WristerEngine::TimedCall>(
		std::bind(&Enemy::FireAndReset, this), FIRE_INTERVAL)));
}

void Enemy::ChangeState(std::unique_ptr<BaseEnemyState> nextState)
{
	nextState->SetEnemy(this);
	state = std::move(nextState);
}
