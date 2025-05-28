#pragma once
#include "BaseCharacter.h"

class Enemy : public BaseCharacter
{
private:
	float walkingParameter = 0.0f;
	int cycle = 60;
	float amplitude = 0.25f;

	void Move();
	void InitializeWalkingGimmick();
	void UpdateWalkingGimmick();

public:
	void Initialize(const std::string& modelGroupName) override;
	void Update() override;
};

