#include "BaseCamera.h"
#include "D3D12Common.h"
#include <array>
using namespace WristerEngine;
using namespace _3D;

void BaseCamera::Initialize()
{
	CreateBuffer(constBuffer.GetAddressOf(), &constMap, (sizeof(ConstBufferData) + 0xff) & ~0xff);
}

void BaseCamera::Update()
{
	UpdateProjectionMatrix();

	// ƒVƒFƒCƒN‚ðŒvŽZ
	sTarget = target;
	sEye = eye;
	if (shake)
	{
		Vector3 shakeVal = shake->Update();
		sTarget += shakeVal;
		sEye += shakeVal;
	}

	VirtualUpdate();

	// 2s—ñ‚ðŠ|‚¯‚é
	matViewProjection = matView * matProjection;

	// GPU“]‘—
	constMap->viewproj = matViewProjection;
	constMap->cameraPos = sEye;
}

void BaseCamera::UpdateProjectionMatrix()
{
	matProjection = Matrix4::Zero();
	matProjection.m[0][0] = 1.0f / (aspectRatio * std::tan(fovAngleY / 2.0f));
	matProjection.m[1][1] = 1.0f / std::tan(fovAngleY / 2.0f);
	matProjection.m[2][2] = farZ / (farZ - nearZ);
	matProjection.m[2][3] = 1.0f;
	matProjection.m[3][2] = -nearZ * farZ / (farZ - nearZ);
}

void BaseCamera::CreateShake(const Shake::Prop& shakeProp)
{
	shake = Shake::Create(shakeProp);
}

void BaseCamera::CameraMove(const Vector3& move)
{
	eye += move;
	target += move;
}