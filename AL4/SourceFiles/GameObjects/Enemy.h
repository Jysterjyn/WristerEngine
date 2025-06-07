#pragma once
#include "BaseCharacter.h"
#include <LockOn.h>

class Enemy : public BaseCharacter, public WE::_3D::LockOnObject
{
private:
	float walkingParameter = 0.0f;
	int cycle = 30;
	float amplitude = 0.25f;
	uint32_t serialNumber = 0;
	static uint32_t nextSerialNumber;

	void Move();
	void InitializeWalkingGimmick();
	void UpdateWalkingGimmick();

public:
	Enemy();
	void Initialize(const std::string& modelGroupName) override;
	void Update() override;
	void SetPosition(const Vector3& position) { rootPos.translation = position; }
	Vector3 GetCenterPos() const override;
	uint32_t GetSerialNumber() const { return serialNumber; }
};

