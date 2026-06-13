#pragma once
#include "GameObjects.h"
#include <Input.h>
#include <Collider2D.h>

class Player : public GameObjects, public WE::_2D::TestCircleCollider
{
	WE::Input* input = WE::Input::GetInstance();

	// GameObjects を介して継承されました
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void OnCollision(TestCircleCollider* other) override;
};