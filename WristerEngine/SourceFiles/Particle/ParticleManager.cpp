#include "ParticleManager.h"
#include "Sprite.h"
#include "D3D12Common.h"
#include "PipelineManager.h"
#include <CameraManager.h>
using namespace Microsoft::WRL;
using namespace WE;
using namespace _3D;

ParticleManager* ParticleManager::GetInstance()
{
	static ParticleManager instance;
	return &instance;
}

void ParticleManager::Initialize()
{
	CreateBuffer(&constBuff, &constMap, (sizeof(ConstBufferData) + 0xff) & ~0xff);
}

void ParticleManager::Update()
{
	for (auto& particleGroup : particleGroups)
	{
		for (auto& particles : particleGroup.second) { particles->Update(); }
	}
	// 定数バッファへデータ転送
	const BaseCamera* camera = CameraManager::GetInstance()->Get();
	constMap->mat = camera->GetViewProjectionMatrix();
	constMap->matBillboard = camera->GetBillboard();
}

static const PipelineType ConvertType(ParticleType particleType)
{
	switch (particleType)
	{
	case WristerEngine::ParticleType::Light:
	default:
		return PipelineType::LightParticle;
	case WristerEngine::ParticleType::Dark:
		return PipelineType::DarkParticle;
	}
}

void ParticleManager::Draw()
{
	for (auto& particleGroup : particleGroups)
	{
		if (particleGroup.second.empty()) { continue; }
		// コマンドリストをセット
		ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
		// プリミティブ形状を設定
		PipelineManager::SetPipeline(ConvertType(particleGroup.first));
		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
		// 定数バッファビューをセット
		cmdList->SetGraphicsRootConstantBufferView(1, constBuff->GetGPUVirtualAddress());
		for (auto& particles : particleGroup.second) { particles->Draw(); }
	}
}

void ParticleManager::Clear()
{
	for (auto& particleGroup : particleGroups)
	{
		for (auto& particles : particleGroup.second) { particles->Clear(); }
	}
}

void ParticleManager::AddParticleGroup(const std::string& textureName, ParticleType particleType)
{
	std::unique_ptr<ParticleGroup> pGroup = std::make_unique<ParticleGroup>();
	pGroup->Initialize(textureName);
	particleGroups[particleType].push_back(std::move(pGroup));
}