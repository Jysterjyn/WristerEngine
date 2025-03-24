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

	VirtualUpdate();

	Vector3 cameraPos = UpdateViewMatrix();

	// 2�s����|����
	matViewProjection = matView * matProjection;

	// GPU�]��
	constMap->viewproj = matViewProjection;
	constMap->cameraPos = cameraPos;
}

Vector3 BaseCamera::UpdateViewMatrix()
{
	// �V�F�C�N���v�Z
	Vector3 shakeVal;
	if (shake) { shakeVal = shake->Update(); }

	// ���[���h�s�񂩂�r���[�s����v�Z
	if (pTransform)
	{
		// �V�F�C�N�̒l�����ʒu�����炷
		Matrix4 sMatWorld = pTransform->matWorld * Matrix4::Translate(shakeVal);
		matView = Inverse(sMatWorld);
		return sMatWorld.GetVector(3);
	}
	// eye, target, up����s����v�Z
	Vector3 sTarget = target + shakeVal;
	Vector3 sEye = eye + shakeVal;
	std::array<Vector3, 3> axis = CalculateAxis(sTarget - sEye, &up);
	Vector3 cameraMove;
	for (size_t i = 0; i < axis.size(); i++) { cameraMove[i] = Dot(sEye, axis[i]); }
	matView = CreateFromVector(axis[(int)Axis::X], axis[(int)Axis::Y], axis[(int)Axis::Z]);
	matView = Inverse(matView);
	matView.SetVector(-cameraMove, 3);
	return sEye;
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

Matrix4 BaseCamera::GetBillboard() const
{
	Vector3 cameraAxisZ = target - eye;
	// 0�x�N�g���̎�
	assert(!(cameraAxisZ.Length() == 0));
	assert(!(up.Length() == 0));

	cameraAxisZ.Normalize();

	Vector3 cameraAxisX = Normalize(Cross(up, cameraAxisZ));
	Vector3 cameraAxisY = Normalize(Cross(cameraAxisZ, cameraAxisX));
	return CreateFromVector(cameraAxisX, cameraAxisY, cameraAxisZ);
}