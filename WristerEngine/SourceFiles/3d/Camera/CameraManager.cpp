#include "CameraManager.h"
using namespace WristerEngine::_3D;

std::unordered_map<std::string, std::unique_ptr<BaseCamera>> CameraManager::cameras;

CameraManager* CameraManager::GetInstance()
{
	static CameraManager instance;
	return &instance;
}

BaseCamera* CameraManager::Create(const std::string& name_, CameraType type, CameraProp* prop)
{
	std::unique_ptr<BaseCamera> newCamera;
	switch (type)
	{
	case CameraType::Normal:
		newCamera = std::make_unique<BaseCamera>();
		break;
	case CameraType::Debug:
		if (prop) { newCamera = std::make_unique<DebugCamera>(&prop->debugProp); }
		else { newCamera = std::make_unique<DebugCamera>(); }
		break;
	case CameraType::Rail:
		if (prop) { newCamera = std::make_unique<RailCamera>(&prop->railProp); }
		else { newCamera = std::make_unique<RailCamera>(); }
		break;
	}

	newCamera->Initialize();
	cameras[name_] = std::move(newCamera);
	SetName(name_);
	return cameras[name_].get();
}

void WristerEngine::_3D::CameraManager::SetName(const std::string& name_)
{
	assert(cameras.contains(name_));
	name = name_;
}
