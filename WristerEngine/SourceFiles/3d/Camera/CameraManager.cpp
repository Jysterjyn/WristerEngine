#include "CameraManager.h"
#include <DebugCamera.h>
#include <RailCamera.h>
#include <FollowCamera.h>
#include <DirectXCommon.h>
using namespace WE::_3D;

std::unordered_map<std::string, std::unique_ptr<BaseCamera>> CameraManager::cameras;

CameraManager* CameraManager::GetInstance()
{
	static CameraManager instance;
	return &instance;
}

template<class T>
std::unique_ptr<BaseCamera> CreateNewCamera(BaseCameraProp* prop)
{
	T::Prop* castProp = dynamic_cast<T::Prop*>(prop);
	return std::make_unique<T>(castProp);
}

BaseCamera* CameraManager::Create(const std::string& name_, CameraType type, BaseCameraProp* prop)
{
	std::unique_ptr<BaseCamera> newCamera;
	switch (type)
	{
	case CameraType::Normal:
		newCamera = std::make_unique<BaseCamera>();
		break;

	case CameraType::Debug:
		newCamera = CreateNewCamera<DebugCamera>(prop);
		break;

	case CameraType::Rail:
		newCamera = CreateNewCamera<RailCamera>(prop);
		break;

	case CameraType::Follow:
		newCamera = CreateNewCamera<FollowCamera>(prop);
		break;
	}

	newCamera->Initialize();
	cameras[name_] = std::move(newCamera);
	Change(name_);
	return cameras[name_].get();
}

void CameraManager::Draw(UINT rootParameterIndex) const
{
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
	cmdList->SetGraphicsRootConstantBufferView(rootParameterIndex, Get()->constBuffer->GetGPUVirtualAddress());
}

void CameraManager::Change(const std::string& name_)
{
	assert(cameras.contains(name_));
	name = name_;
}
