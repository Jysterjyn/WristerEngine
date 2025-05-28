#pragma once
#include <ModelManager.h>
#include <unordered_map>
#include <Input.h>

class Player
{
	std::unordered_map<std::string, WE::_3D::Object3d*> objects;
	WE::_3D::Transform rootPos;
	WE::Input* input = WE::Input::GetInstance();
	const WE::_3D::BaseCamera* camera = nullptr;
	float goalAngle = 0;
	float floatingParameter = 0.0f;
	int cycle = 60;
	float amplitude = 0.25f;

	void Move();
	void InitializeFloatingGimmick();
	void UpdateFloatingGimmick();

public:
	void Initialize();
	void Update();
	void Draw();
	void SetCamera(){ camera = WE::_3D::CameraManager::GetInstance()->Get(); }
	const WE::_3D::Transform* GetTransform() const { return &rootPos; }
};
