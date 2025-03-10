#include "Enemy.h"
using namespace WristerEngine::_3D;

void Enemy::Initialize()
{
	obj = ModelManager::GetInstance()->Create("enemy1");
	obj->transform.scale *= 0.5f;
	obj->transform.translation = { 0,2,40 };

	std::unique_ptr<EnemyStateApproach> nextState;
	nextState = std::make_unique<EnemyStateApproach>();
	ChangeState(std::move(nextState));
}

void Enemy::Update()
{
	state->Update();
}

void Enemy::ChangeState(std::unique_ptr<BaseEnemyState> nextState)
{
	nextState->SetEnemy(this);
	state = std::move(nextState);
}
