#pragma once
#include <numeric>
#include <unordered_map>
#include <string>
#include <GlobalVariables.h>
#include <Input.h>
#include <optional>
#include <LockOn.h>
#include <ModelManager.h>
#include <CollisionManager.h>
#include <ContactRecord.h>

class BaseBehavior
{
public:
	enum class Behavior
	{
		Root, Attack, Dash, Jump
	};

protected:
	static std::unordered_map<std::string, WE::_3D::Object3d*>* objects;
	static WE::_3D::Transform* rootPos;
	static const WE::_3D::BaseCamera* camera;
	static float destinationAngleY;
	static Vector3 velocity;
	static WE::GlobalVariables* globalVariables;
	static WE::Input* input;
	static const WE::_3D::LockOn* lockOn;
	// Ÿ‚ÌU‚é•‘‚¢ƒŠƒNƒGƒXƒg
	static std::optional<Behavior> behaviorRequest;

	float moveSpeed = 0.3f;
	std::string groupName = "Player";

	void Move();

public:
	virtual ~BaseBehavior() = default;
	static void SetObjects(std::unordered_map<std::string, WE::_3D::Object3d*>* objects_) { objects = objects_; }
	static void SetTransform(WE::_3D::Transform* rootPos_) { rootPos = rootPos_; }
	static void SetCamera() { camera = WE::_3D::CameraManager::GetInstance()->Get(); }
	static void SetBehaviorRequest(std::optional<Behavior> nextBehavior) { behaviorRequest = nextBehavior; }
	static void SetLockOn(const WE::_3D::LockOn* lockOn_) { lockOn = lockOn_; }
	static std::optional<Behavior> GetBehaviorRequest() { return behaviorRequest; }
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void ApplyGlobalVariables() {}
};

class RootBehavior : public BaseBehavior
{
	float parameter = 0.0f;
	int cycle = 0;
	float amplitude = 0.0f;
	float idleArmAngleMax = 0;

	// BaseBehavior ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	void Initialize() override;
	void Update() override;
	void ApplyGlobalVariables() override;
};

class Sword : public WE::SphereCollider
{
private:
	WE::_3D::Object3d* object = nullptr;
	ContactRecord contactRecord;

public:
	Sword()
	{
		object = WE::_3D::ModelManager::GetInstance()->Create("Sword"); 
		pTransform = &object->transform;
		debugObject->isInvisible = !WE::CollisionManager::IsPrint();
		collisionAttribute = CollisionAttribute::PlayerWeapon;
		object->material.ambient = { 0,0,0 };
	}
	~Sword() { object->isDestroy = true; }
	void SetParent(WE::_3D::Transform* parent) { object->transform.parent = parent; }
	const Vector3& GetRotation() const { return object->transform.rotation; }
	void SetRotation(const Vector3& rotation) { object->transform.rotation = rotation; }
	void OnCollision(WE::SphereCollider* collider) override;
	void Update()
	{
		debugObject->transform = *pTransform;
	}
	void ClearRecord() { contactRecord.Clear(); }
};

class AttackBehavior : public BaseBehavior
{
public:
	static const int ComboNum = 3;

private:
	struct ConstAttack
	{
		uint32_t anticipationTime;
		uint32_t chargeTime;
		uint32_t swingTime;
		uint32_t recoveryTime;
		float anticipationSpeed;
		float chargeSpeed;
		float swingSpeed;

		uint32_t GetComboTime(uint32_t inComboPhase = 3) const
		{
			uint32_t sumTime = 0;
			sumTime += anticipationTime;
			if (inComboPhase >= 1) { sumTime += chargeTime; }
			if (inComboPhase >= 2) { sumTime += swingTime; }
			if (inComboPhase >= 3) { sumTime += recoveryTime; }
			return sumTime;
		}
	};

	static const std::array<ConstAttack, ComboNum> kConstAttacks;
	float attackParameter = 0;
	uint32_t parameter = 0;
	uint32_t comboIndex = 0;
	uint32_t inComboPhase = 0;
	bool comboNext = false;
	Sword sword;

	// BaseBehavior ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	void Initialize() override;
	void Update() override;
	void ApplyGlobalVariables() override;

	void Combo1();
	void Combo2();
	void Combo3();
};

class DashBehavior : public BaseBehavior
{
	uint32_t parameter = 0;

	// BaseBehavior ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	void Initialize() override;
	void Update() override;
};

class JumpBehavior : public BaseBehavior
{
	uint32_t parameter = 0;

	// BaseBehavior ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	void Initialize() override;
	void Update() override;
};
