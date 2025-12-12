#pragma once
#include <Behavior.h>
#include <SpriteManager.h>

class GameObjects
{
private:
	bool isDestroy = false;

protected:
	WE::BehaviorPattern behavior;
	WE::_2D::Sprite* sprite = nullptr;
	WE::_2D::SpriteManager* spMan = WE::_2D::SpriteManager::GetInstance();
	WE::BehaviorPattern behaviorPattern;

	void Destroy() { isDestroy = true; }

public:
	virtual ~GameObjects() = default;
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	bool IsDestroy() const { return isDestroy; }
};

