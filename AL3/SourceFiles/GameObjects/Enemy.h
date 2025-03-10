#pragma once
#include <ModelManager.h>
#include <EnemyState.h>

class Enemy
{
	WristerEngine::_3D::Object3d* obj = nullptr;
	std::unique_ptr<BaseEnemyState> state;

public:
	void Initialize();
	void Update();
	void ChangeState(std::unique_ptr<BaseEnemyState> nextState);
	void Move(const Vector3& velocity) { obj->transform.translation += velocity; }
	const Vector3& GetPosition() const { return obj->transform.translation; }
};

