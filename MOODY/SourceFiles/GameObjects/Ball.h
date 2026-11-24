#pragma once
#include <GameObjects.h>
#include <Collider2D.h>
#include <TimedCall.h>

struct BehaviorParameter : WE::BehaviorParameter
{
	Vector2 pos;
	WE::ColorRGB color;
	bool isUkenagashi = false;
	float rotation = 0;
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

class Ball : public GameObjects, public WE::QueryCallback
{
	static WE::ColliderGroup* group;
	WE::_2D::CircleCollider* collider = nullptr;

	// GameObjects を介して継承されました
	void Initialize() override;
	void Update() override;
	void Draw() override { sprite->Draw(); }

	template<class T>
	void CreateBehavior()
	{
		WE::BehaviorPair behavior;
		behavior.first = std::make_unique<T>();
		behavior.first->Initialize();
		behaviorPattern.Add(behavior);
	}

	void ApplyParameter(); // パラメータ適用
	BehaviorParameter* GetParameter() { return static_cast<BehaviorParameter*>(behaviorPattern.GetParam()); }

public:
	~Ball() { collider->Destroy(); }
	static void SetColliderGroup(WE::ColliderGroup* colliderGroup) { group = colliderGroup; }
	bool IsUkenagashi();
	// 次のビヘイビアに切り替え
	void ChangeNextBehavior(WE::CR<Vector2> pos);
	// QueryCallback を介して継承されました
	void OnQueryHit(WE::CollisionPair* info) override;
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
