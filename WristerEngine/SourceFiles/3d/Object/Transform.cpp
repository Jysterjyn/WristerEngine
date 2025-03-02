#include "Transform.h"
#include "D3D12Common.h"
#include "ModelManager.h"
using namespace WristerEngine;
using namespace _3D;

void Transform::Initialize()
{
	CreateBuffer(constBuffer.GetAddressOf(), &constMap, (sizeof(ConstBufferData) + 0xff) & ~0xff);
}

void Transform::Update()
{
	// 親ポインタの更新
	if (parent) { parent->Update(); }
	// 更新済なら関数を抜ける
	if (isUpdated) { return; }
	Matrix4 matScale = Matrix4::Scale(scale); // スケール行列
	Matrix4 matRot = Matrix4::Rotate(rotation); // 回転行列
	matWorld = matScale * matRot;
	matWorld.SetVector(translation, 3); // 平行移動成分を代入
	if (parent) { matWorld *= parent->matWorld; } // 親のワールド行列を掛ける
	isUpdated = true;
	constMap->world = matWorld;
}

void Transform::Draw()
{
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
	cmdList->SetGraphicsRootConstantBufferView(
		(UINT)RootParamNum::MatWorld, constBuffer->GetGPUVirtualAddress());
	isUpdated = false;
}

Vector3 Transform::GetScale() const
{
	Vector3 scale_;
	// グローバル行列からスケール値を抽出
	for (size_t i = 0; i < 3; i++) { scale_[i] = matWorld.GetVector(i).Length(); }
	return scale_;
}
