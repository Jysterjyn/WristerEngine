#pragma once
#include "BaseCharacter.h"
#include <LockOn.h>

class Enemy : public BaseCharacter, public WE::_3D::LockOnObject
{
private:
	float walkingParameter = 0.0f;
	int cycle = 30;
	float amplitude = 0.25f;

	void Move();
	void InitializeWalkingGimmick();
	void UpdateWalkingGimmick();

public:
	void Initialize(const std::string& modelGroupName) override;
	void Update() override;
	void SetPosition(const Vector3& position) { rootPos.translation = position; }
	Vector3 GetCenterPos() const override;
};

