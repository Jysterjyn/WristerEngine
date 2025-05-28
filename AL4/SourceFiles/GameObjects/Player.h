#pragma once
#include <ModelManager.h>
#include <unordered_map>
#include <Input.h>
#include <BaseCharacter.h>

class Player : public BaseCharacter
{
	WE::Input* input = WE::Input::GetInstance();
	const WE::_3D::BaseCamera* camera = nullptr;
	float goalAngle = 0;
	float floatingParameter = 0.0f;
	int cycle = 60;
	float amplitude = 0.25f;

	void Move();
	void InitializeFloatingGimmick();
	void UpdateFloatingGimmick();

public:
	void Initialize(const std::string& modelGroupName) override;
	void Update() override;
	void SetCamera(){ camera = WE::_3D::CameraManager::GetInstance()->Get(); }
};
