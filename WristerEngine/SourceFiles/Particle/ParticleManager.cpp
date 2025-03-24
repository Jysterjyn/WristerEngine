#include "ParticleManager.h"
#include "Sprite.h"
#include "D3D12Common.h"
#include "PipelineManager.h"
using namespace Microsoft::WRL;
using namespace WristerEngine;
using namespace _3D;

// �ÓI�����o�ϐ��̎���
ComPtr<ID3D12Resource> ParticleManager::constBuff;
ParticleManager::ConstBufferData* ParticleManager::constMap = nullptr;
std::vector<ParticleGroup> ParticleManager::particleGroups;
ModelManager* ParticleManager::modelManager = ModelManager::GetInstance();

void ParticleManager::Initialize()
{
	CreateBuffer(&constBuff, &constMap, (sizeof(ConstBufferData) + 0xff) & ~0xff);
}

void ParticleManager::Update()
{
	for (auto& particleGroup : particleGroups) { particleGroup.Update(); }
	// �萔�o�b�t�@�փf�[�^�]��
	BaseCamera* camera = modelManager->GetCamera();
	constMap->mat = camera->GetViewProjectionMatrix();
	constMap->matBillboard = camera->GetBillboard();
}

void ParticleManager::Draw()
{
	// �R�}���h���X�g���Z�b�g
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
	PipelineManager::SetPipeline(PipelineType::Particle);
	// �v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	// �萔�o�b�t�@�r���[���Z�b�g
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