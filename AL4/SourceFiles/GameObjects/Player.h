#pragma once
#include <ModelManager.h>
#include <unordered_map>
#include <BaseCharacter.h>
#include <PlayerBehavior.h>

class Player : public BaseCharacter
{
private:
	std::string groupName = "Player";
	WE::GlobalVariables* globalVariables = WE::GlobalVariables::GetInstance();
	std::unique_ptr<BaseBehavior> pBehavior;

	void ApplyGlobalVariables();

public:	
	void Initialize(const std::string& modelGroupName) override;
	void Update() override;
	void OnCollision(WE::SphereCollider* collider) override;
};
