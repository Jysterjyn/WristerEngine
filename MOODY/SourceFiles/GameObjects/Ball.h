#pragma once
#include <GameObjects.h>
#include <Vector.h>
#include <Collider2D.h>

struct BehaviorParameter : WE::Behavior::Parameter
{
	Vector2 pos;
	WE::ColorRGB color;
	bool isDestroy = false;
	bool isUkenagashi = false;
};

class BaseBallBehavior : public WE::Behavior
{
protected:
	BehaviorParameter* pParam = nullptr;
};

class Move : public BaseBallBehavior
{
	// Behavior を介して継承されました
	void Initialize() override;
	void Update() override;
};

class Ukenagashi : public BaseBallBehavior
{
	// Behavior を介して継承されました
	void Initialize() override;
	void Update() override;
};

class Ball : public GameObjects, public WE::_2D::Collider
{
	BehaviorParameter* GetParameter();
	WE::_2D::CircleCollider* collider = nullptr;

	// GameObjects を介して継承されました
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void OnCollision() override;

	template<class T>
	void CreateBehavior()
	{
		WE::BehaviorPair behavior;
		behavior.first = std::make_unique<T>();
		behavior.first->Initialize();
		behaviorPattern.Add(behavior);
	}

	void ApplyParameter(); // パラメータ適用
};

