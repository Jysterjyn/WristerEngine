#pragma once
#include <ModelManager.h>
#include <unordered_map>
#include <Input.h>
#include <BaseCharacter.h>
#include <optional>

class Player : public BaseCharacter
{
	enum class Behavior
	{
		Root,Attack
	};

	WE::Input* input = WE::Input::GetInstance();
	const WE::_3D::BaseCamera* camera = nullptr;
	float goalAngle = 0;
	float floatingParameter = 0.0f;
	int cycle = 60;
	float amplitude = 0.25f;
	Behavior behavior = Behavior::Root;
	// 次の振る舞いリクエスト
	std::optional<Behavior> behaviorRequest = std::nullopt;

	void Move();
	void InitializeFloatingGimmick();
	void UpdateFloatingGimmick();
	void BehaviorRootInitialize();
	void BehaviorRootUpdate();
	void BehaviorAttackInitialize();
	void BehaviorAttackUpdate();

public:
	void Initialize(const std::string& modelGroupName) override;
	void Update() override;
	void SetCamera(){ camera = WE::_3D::CameraManager::GetInstance()->Get(); }
};
