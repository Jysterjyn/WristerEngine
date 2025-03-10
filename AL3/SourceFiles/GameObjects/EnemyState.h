#pragma once
class Enemy;

class BaseEnemyState
{
protected:
	Enemy* enemy = nullptr;

public:
	void SetEnemy(Enemy* enemy_) { enemy = enemy_; }
	virtual ~BaseEnemyState() = default;
	virtual void Update() = 0;
};

class EnemyStateApproach : public BaseEnemyState
{
	// BaseEnemyState ����Čp������܂���
	void Update() override;
};

class EnemyStateLeave : public BaseEnemyState
{
	// BaseEnemyState ����Čp������܂���
	void Update() override;
};