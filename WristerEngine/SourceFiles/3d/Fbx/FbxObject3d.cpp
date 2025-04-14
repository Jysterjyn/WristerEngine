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
	// 定数バッファの生成
	CreateBuffer(&constBuff, &constMap, (sizeof(ConstBufferData) + 0xff) & ~0xff);
	CreateBuffer(&constBuffSkin, &constMapSkin, (sizeof(ConstBufferDataSkin) + 0xff) & ~0xff);

	transform = transform_;
	transform->Initialize();
	model = model_;

	for (int i = 0; i < MAX_BONES; i++) { constMapSkin->bones[i] = Matrix4::Identity(); }

	// 1フレーム分の時間を60FPSで設定
	frameTime.SetTime(0, 0, 0, 1, 0, FbxTime::EMode::eFrames60);
	PlayAnimation();
}

void FbxObject3d::Update()
{
	// スケール、回転、平行移動行列の計算
	transform->Update();

	// アニメーション
	if (isPlay)
	{
		// 1フレーム進める
		currentTime += frameTime;
		// 最後まで再生したら先頭に戻す
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
	// 定数バッファへデータ転送
	constMap->viewproj = modelManager->GetCamera()->GetViewProjectionMatrix();
	constMap->world = model->GetModelTransform() * transform->matWorld;
	constMap->cameraPos = modelManager->GetCamera()->eye;
}

void FbxObject3d::Draw()
{
	// モデルの割り当てがなければ描画しない
	if (model == nullptr) { return; }

	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
	PipelineManager::SetPipeline(PipelineType::Fbx);
	// プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(3, constBuff->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootConstantBufferView(4, constBuffSkin->GetGPUVirtualAddress());
	lightGroup->Draw(6);
	// モデル描画
	model->Draw();
}

void FbxObject3d::PlayAnimation()
{
	FbxScene* fbxScene = model->GetFbxScene();
	// 0番のアニメーション取得
	FbxAnimStack* animstack = fbxScene->GetSrcObject<FbxAnimStack>(0);
	// アニメーションの名前取得
	const char* animstackname = animstack->GetName();
	// アニメーションの時間取得
	FbxTakeInfo* takeinfo = fbxScene->GetTakeInfo(animstackname);
	// 開始時間取得
	startTime = takeinfo->mLocalTimeSpan.GetStart();
	// 終了時間取得
	endTime = takeinfo->mLocalTimeSpan.GetStop();
	// 開始時間に合わせる
	currentTime = startTime;
	// 再生中状態にする
	isPlay = true;
}
