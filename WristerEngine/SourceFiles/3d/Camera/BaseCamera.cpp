#include "BaseCamera.h"
#include "D3D12Common.h"
#include <array>
using namespace WristerEngine;
using namespace _3D;

void Projection::UpdateMatrix()
{
	matProjection = Matrix4::Zero();
	matProjection.m[0][0] = 1.0f / (aspectRatio * std::tan(fovAngleY / 2.0f));
	matProjection.m[1][1] = 1.0f / std::tan(fovAngleY / 2.0f);
	matProjection.m[2][2] = farZ / (farZ - nearZ);
	matProjection.m[2][3] = 1.0f;
	matProjection.m[3][2] = -nearZ * farZ / (farZ - nearZ);
}

void BaseCamera::Initialize()
{
	CreateBuffer(constBuffer.GetAddressOf(), &constMap, (sizeof(ConstBufferData) + 0xff) & ~0xff);
}

void BaseCamera::Update()
{
	// プロジェクション行列の更新
	Projection::UpdateMatrix();
	// 固有の更新
	VirtualUpdate();
	// 全カメラ共通の更新処理
	UpdateViewMatrix();
	// 2行列を掛ける
	matViewProjection = matView * GetProjectionMatrix();
	// GPU転送
	constMap->viewproj = matViewProjection;
	constMap->cameraPos = cameraPos;
}

void BaseCamera::UpdateViewMatrix()
{
	// シェイクを計算
	Vector3 shakeVal;
	if (shake) { shakeVal = shake->Update(); }

	// ワールド行列からビュー行列を計算
	if (calMode == CalMode::Transform)
	{
		assert(pTransform);
		// シェイクの値だけ位置をずらす
		Matrix4 sMatWorld = pTransform->matWorld * Matrix4::Translate(shakeVal);
		matView = Inverse(sMatWorld);
		cameraPos = sMatWorld.GetVector(3);
		// eye, targetに変換
		eye = pTransform->GetWorldPosition();
		target = Vector3(0, 0, 1) * Matrix4::Rotate(pTransform->rotation);
		return;
	}

	// eye, target, upから行列を計算
	Vector3 sTarget = target + shakeVal;
	Vector3 sEye = eye + shakeVal;
	std::array<Vector3, 3> axis = CalculateAxis(sTarget - sEye, &up);
	Vector3 cameraMove;
	for (size_t i = 0; i < axis.size(); i++) { cameraMove[i] = Dot(sEye, axis[i]); }
	matView = CreateFromVector(axis[(int)Axis::X], axis[(int)Axis::Y], axis[(int)Axis::Z]);
	matView = Inverse(matView);
	matView.SetVector(-cameraMove, 3);
	cameraPos = sEye;
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
	// 0ベクトルの時
	assert(!(cameraAxisZ.Length() == 0));
	assert(!(up.Length() == 0));

	cameraAxisZ.Normalize();

	Vector3 cameraAxisX = Normalize(Cross(up, cameraAxisZ));
	Vector3 cameraAxisY = Normalize(Cross(cameraAxisZ, cameraAxisX));
	return CreateFromVector(cameraAxisX, cameraAxisY, cameraAxisZ);
}
