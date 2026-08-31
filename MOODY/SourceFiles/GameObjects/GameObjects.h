#pragma once
#include <Behavior.h>
#include <SpriteManager.h>

class GameObjects : public WE::ListObject
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
	bool Remove() const override { return isDestroy; }
	//void Draw() override { sprite->Draw(); }
};

