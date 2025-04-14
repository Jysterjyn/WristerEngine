#include "FbxObject3d.h"
#include <PipelineManager.h>
#include <D3D12Common.h>
#include <ModelManager.h>
#include <array>
using namespace Microsoft::WRL;
using namespace DirectX;
using namespace WristerEngine;
using namespace _3D;

LightGroup* FbxObject3d::lightGroup = nullptr;

void FbxObject3d::Initialize(Transform* transform_, FbxModel* model_)
{
	// �萔�o�b�t�@�̐���
	CreateBuffer(&constBuff, &constMap, (sizeof(ConstBufferData) + 0xff) & ~0xff);
	CreateBuffer(&constBuffSkin, &constMapSkin, (sizeof(ConstBufferDataSkin) + 0xff) & ~0xff);

	transform = transform_;
	transform->Initialize();
	model = model_;

	for (int i = 0; i < MAX_BONES; i++) { constMapSkin->bones[i] = Matrix4::Identity(); }

	// 1�t���[�����̎��Ԃ�60FPS�Őݒ�
	frameTime.SetTime(0, 0, 0, 1, 0, FbxTime::EMode::eFrames60);
	PlayAnimation();
}

void FbxObject3d::Update()
{
	// �X�P�[���A��]�A���s�ړ��s��̌v�Z
	transform->Update();

	// �A�j���[�V����
	if (isPlay)
	{
		// 1�t���[���i�߂�
		currentTime += frameTime;
		// �Ō�܂ōĐ�������擪�ɖ߂�
		if (currentTime > endTime) { currentTime = startTime; }
	}

	std::vector<FbxModel::Bone>& bones = model->GetBones();
	for (int i = 0; i < bones.size(); i++)
	{
		FbxAMatrix fbxCurrentPose = bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime);
		Matrix4 matCurrentPose;
		FbxModel::ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);
		constMapSkin->bones[i] = bones[i].invInitialPose * matCurrentPose;
	}

	ModelManager* modelManager = ModelManager::GetInstance();
	// �萔�o�b�t�@�փf�[�^�]��
	constMap->viewproj = modelManager->GetCamera()->GetViewProjectionMatrix();
	constMap->world = model->GetModelTransform() * transform->matWorld;
	constMap->cameraPos = modelManager->GetCamera()->eye;
}

void FbxObject3d::Draw()
{
	// ���f���̊��蓖�Ă��Ȃ���Ε`�悵�Ȃ�
	if (model == nullptr) { return; }

	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
	PipelineManager::SetPipeline(PipelineType::Fbx);
	// �v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// �萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(3, constBuff->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootConstantBufferView(4, constBuffSkin->GetGPUVirtualAddress());
	lightGroup->Draw(6);
	// ���f���`��
	model->Draw();
}

void FbxObject3d::PlayAnimation()
{
	FbxScene* fbxScene = model->GetFbxScene();
	// 0�Ԃ̃A�j���[�V�����擾
	FbxAnimStack* animstack = fbxScene->GetSrcObject<FbxAnimStack>(0);
	// �A�j���[�V�����̖��O�擾
	const char* animstackname = animstack->GetName();
	// �A�j���[�V�����̎��Ԏ擾
	FbxTakeInfo* takeinfo = fbxScene->GetTakeInfo(animstackname);
	// �J�n���Ԏ擾
	startTime = takeinfo->mLocalTimeSpan.GetStart();
	// �I�����Ԏ擾
	endTime = takeinfo->mLocalTimeSpan.GetStop();
	// �J�n���Ԃɍ��킹��
	currentTime = startTime;
	// �Đ�����Ԃɂ���
	isPlay = true;
}
