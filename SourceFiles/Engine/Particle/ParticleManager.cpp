#include "ParticleManager.h"
#include "Sprite.h"
#include "D3D12Common.h"
#include "ModelManager.h"
using namespace Microsoft::WRL;
using namespace WristerEngine;

// 静的メンバ変数の実体
ComPtr<ID3D12Resource> ParticleManager::constBuff;
ParticleManager::ConstBufferData* ParticleManager::constMap = nullptr;
std::vector<ParticleGroup> ParticleManager::particleGroups;

void ParticleManager::Initialize()
{	
	CreateBuffer(&constBuff, &constMap, (sizeof(ConstBufferData) + 0xff) & ~0xff);
}

void ParticleManager::Update()
{
	for (auto& particleGroup : particleGroups) { particleGroup.Update(); }
	// 定数バッファへデータ転送
	constMap->mat = _3D::ModelManager::GetViewProjection()->GetViewProjectionMatrix();
	constMap->matBillboard = Matrix4::GetBillboard();
}

void ParticleManager::Draw()
{
	// コマンドリストをセット
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
	PipelineManager::SetPipeline(PipelineType::Particle);
	// プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(1, constBuff->GetGPUVirtualAddress());
	for (auto& particleGroup : particleGroups) { particleGroup.Draw(); }
}

void ParticleManager::Clear()
{
	for (auto& particleGroup : particleGroups) { particleGroup.Clear(); }
}

void ParticleManager::AddParticleGroup(const std::string& textureName)
{
	ParticleGroup pGroup;
	pGroup.Initialize(textureName);
	particleGroups.push_back(pGroup);
}