#pragma once
#include <Sprite.h>
#include <CameraManager.h>

class LockOnObject
{
public:
	virtual ~LockOnObject() = default;
	virtual void Update() = 0;
	virtual Vector3 GetCenterPos() const = 0;
};

class LockOn
{
	WE::_2D::Sprite* lockOnMark = nullptr;
	const LockOnObject* target = nullptr;
	const WE::_3D::BaseCamera* camera = nullptr;
	const std::list<std::unique_ptr<LockOnObject>>* objects;

	float minDistance = 10.0f;
	float maxDistance = 30.0f;
	float angleRange = Angle(20);

	void Search();
	bool IsOutRange(const LockOnObject* object, float* positionViewZ = nullptr);

public:
	void Initialize(std::initializer_list<const std::string> textureNames);
	void Update();
	void Draw();
	void SetLockOnObjects(const std::list<std::unique_ptr<LockOnObject>>* objects_) { objects = objects_; }
	Vector3 GetTargetPosition() const;
	bool ExistTarget() const { return target ? true : false; }
};
