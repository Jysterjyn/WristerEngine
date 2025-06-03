#pragma once
#include <Sprite.h>
#include <Enemy.h>
#include <CameraManager.h>

class LockOn
{
	WE::_2D::Sprite* lockOnMark = nullptr;
	const Enemy* target = nullptr;
	const WE::_3D::BaseCamera* camera = nullptr;

	float minDistance = 10.0f;
	float maxDistance = 30.0f;
	float angleRange = Angle(20);

	void Search(const std::list<std::unique_ptr<Enemy>>& enemies);
	bool IsOutRange();

public:
	void Initialize();
	void Update(const std::list<std::unique_ptr<Enemy>>& enemies);
	void Draw();
};
