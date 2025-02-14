#include "ModelManager.h"
#include "Sprite.h"
#include "D3D12Common.h"
using namespace Microsoft::WRL;
using namespace std;
using namespace WristerEngine;
using namespace _3D;

// 静的メンバ変数の実体
unique_ptr<LightGroup> ModelManager::lightGroup;
unordered_map<string, array<unique_ptr<Mesh>, 2>> ModelManager::meshes;
list<unique_ptr<Object3d>> ModelManager::objects;
Camera* ModelManager::camera = nullptr;

void ModelManager::Initialize()
{
	// ライトグループ生成
	lightGroup = LightGroup::Create();
	// カメラ生成
	camera = new Camera;
	camera->Initialize();
}

Object3d* ModelManager::Create(const string& modelName, bool smoothing)
{
	unique_ptr<Object3d> newObj3d = make_unique<Object3d>();

	// モデルの再読み込みをチェック
	Mesh* mesh = meshes[modelName][smoothing].get();
	if (mesh)
	{
		// 既に読み込んでいたモデルの場合
		newObj3d->Initialize(mesh);
		objects.push_back(move(newObj3d));
		return objects.back().get();
	}

	unique_ptr<Mesh> newMesh = make_unique<Mesh>();
	newMesh->LoadOBJ(modelName, smoothing);
	newObj3d->Initialize(newMesh.get());
	meshes[modelName][smoothing] = move(newMesh);
	objects.push_back(move(newObj3d));
	return objects.back().get();
}

void ModelManager::DrawObjects()
{
	// コマンドリストをセット
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();

	PipelineManager::SetPipeline(PipelineType::Object);
	// プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// ライトの描画
	lightGroup->Draw((UINT)RootParamNum::Light);
	// カメラ
	cmdList->SetGraphicsRootConstantBufferView((UINT)RootParamNum::Camera, camera->constBuffer->GetGPUVirtualAddress());
	for (auto& object : objects) { object->Draw(); }
}

void ModelManager::Update()
{
	lightGroup->Update();
	camera->Update();
	objects.remove_if([](std::unique_ptr<Object3d>& object) { return object->isDestroy; });
	for (auto& object : objects) { object->Update(); }
}