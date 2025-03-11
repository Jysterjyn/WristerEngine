#pragma once
#include <ModelManager.h>
#include <EnemyState.h>
#include <EnemyBullet.h>
#include <TimedCall.h>

class Player;

class Enemy
{
	static const int FIRE_INTERVAL = 60;

	WristerEngine::_3D::Object3d* obj = nullptr;
	std::unique_ptr<BaseEnemyState> state;
	std::list<std::unique_ptr<EnemyBullet>> bullets;
	std::list<std::unique_ptr<WristerEngine::TimedCall>> timedCalls;
	const Player* player = nullptr;

	void Fire();
	void FireAndReset();

public:
	void Initialize();
	void Update();
	void ChangeState(std::unique_ptr<BaseEnemyState> nextState);
	void Move(const Vector3& velocity) { obj->transform.translation += velocity; }
	void FireReserve();
	void ClearTimedCalls() { timedCalls.clear(); }
	void SetPlayer(Player* player_) { player = player_; }
	const Vector3& GetPosition() const { return obj->transform.translation; }
};

