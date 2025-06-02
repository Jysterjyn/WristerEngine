#pragma once
#include <ModelManager.h>
#include <unordered_map>
#include <Input.h>
#include <BaseCharacter.h>
#include <optional>
#include <GlobalVariables.h>

class Player : public BaseCharacter
{
	enum class Behavior
	{
		Root, Attack, Dash, Jump
	};

	struct WorkDash
	{
		uint32_t dashParameter = 0;
	};

	WE::Input* input = WE::Input::GetInstance();
	const WE::_3D::BaseCamera* camera = nullptr;
	float destinationAngleY = 0.0f;
	float floatingParameter = 0.0f;
	int cycle = 60;
	float amplitude = 0.25f;
	Vector3 velocity;
	WorkDash workDash;
	Behavior behavior = Behavior::Root;
	// 次の振る舞いリクエスト
	std::optional<Behavior> behaviorRequest = std::nullopt;
	WE::GlobalVariables* globalVariables = WE::GlobalVariables::GetInstance();

	void Move();
	void InitializeFloatingGimmick();
	void UpdateFloatingGimmick();
	void BehaviorRootInitialize();
	void BehaviorRootUpdate();
	void BehaviorAttackInitialize();
	void BehaviorAttackUpdate();
	void BehaviorDashInitialize();
	void BehaviorDashUpdate();
	void BehaviorJumpInitialize();
	void BehaviorJumpUpdate();
	void ApplyGlobalVariables();

public:
	void Initialize(const std::string& modelGroupName) override;
	void Update() override;
	void SetCamera() { camera = WE::_3D::CameraManager::GetInstance()->Get(); }
};
