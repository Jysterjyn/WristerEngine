#include "EnemyState.h"
#include <Enemy.h>

void EnemyStateApproach::Initialize()
{
	enemy->FireReserve();
}

void EnemyStateApproach::Update()
{
	enemy->Move({ 0,0,-0.2f });

	if (enemy->GetPosition().z < 0.0f)
	{
		std::unique_ptr<EnemyStateLeave> nextState;
		nextState = std::make_unique<EnemyStateLeave>();
		enemy->ChangeState(std::move(nextState));
	}
}

EnemyStateApproach::~EnemyStateApproach()
{
	enemy->ClearTimedCalls();
}

void EnemyStateLeave::Update()
{
	enemy->Move({ 0.2f, 0.2f, -0.2f });
}
