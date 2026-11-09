#pragma once
#include <GameObjects.h>
#include <Vector.h>
#include <Collider2D.h>
#include <TimedCall.h>
#include <ColliderManager.h>

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

class Ball : public GameObjects
{
	static WE::ColliderGroup* group;
	BehaviorParameter* GetParameter();
	WE::_2D::CircleCollider* collider = nullptr;

	// GameObjects を介して継承されました
	void Initialize() override;
	void Update() override;
	void Draw() override;

	template<class T>
	void CreateBehavior()
	{
		WE::BehaviorPair behavior;
		behavior.first = std::make_unique<T>();
		behavior.first->Initialize();
		behaviorPattern.Add(behavior);
	}

	void ApplyParameter(); // パラメータ適用

public:
	~Ball();
	static void SetColliderGroup(WE::ColliderGroup* colliderGroup) { group = colliderGroup; }
	bool IsUkenagashi() { return GetParameter()->isUkenagashi; }
	void ChangeNextBehavior(WE::CR<Vector2> pos); // 次のビヘイビアに切り替え
};

class BallManager : public WE::_2D::Collider, public WE::ListObject
{
	WE::uList balls;
	WE::uList timedCalls;

	void CreateBall();

public:
	// ListObject を介して継承されました
	void Initialize() override;
	void Update() override;
	void Draw() override { balls.Draw(); }
	void OnCollision() override;
};
