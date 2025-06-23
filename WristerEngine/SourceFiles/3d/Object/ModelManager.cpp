#include "ModelManager.h"
#include "Sprite.h"
#include "D3D12Common.h"
#include "PipelineManager.h"
#include <filesystem>
#include <StringUtility.h>
using namespace Microsoft::WRL;
using namespace WE;
using namespace _3D;

ModelManager* ModelManager::GetInstance()
{
	static ModelManager instance;
	return &instance;
}

void ModelManager::Initialize()
{
	// ライトグループ生成
	lightGroup = LightGroup::Create();
	// カメラ生成
	cameraManager->Create("default");
}

Object3d* ModelManager::Create(const std::string& modelName, bool smoothing, const std::string& directoryName)
{
	std::unique_ptr<Object3d> newObj3d = std::make_unique<Object3d>();

	// モデルの再読み込みをチェック
	Mesh* mesh = meshes[modelName][smoothing].get();
	if (mesh)
	{
		// 既に読み込んでいたモデルの場合
		newObj3d->Initialize(mesh);
		objects.push_back(move(newObj3d));
		return objects.back().get();
	}

	std::unique_ptr<Mesh> newMesh = std::make_unique<Mesh>();
	newMesh->LoadOBJ(modelName, smoothing, directoryName);
	newObj3d->Initialize(newMesh.get());
	meshes[modelName][smoothing] = std::move(newMesh);
	objects.push_back(move(newObj3d));
	return objects.back().get();
}

std::unordered_map<std::string, Object3d*> ModelManager::CreateGroup(const std::string& directoryName, bool smoothing)
{
	std::unordered_map<std::string, Object3d*> newGroup;

	auto dir_it = std::filesystem::directory_iterator(DEFAULT_RESOURCE_PATH + Mesh::DEFAULT_DIRECTORY + directoryName);
	for (auto& p : dir_it)
	{
		std::string modelName = ExtractFileName(p.path().string());
		newGroup[modelName] = Create(modelName, smoothing, directoryName);
	}

	return newGroup;
}

void ModelManager::Draw()
{
	if (objects.empty()) { return; }
	// コマンドリストをセット
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();

	PipelineManager::SetPipeline(PipelineType::Object);
	// プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// ライトの描画
	lightGroup->Draw((UINT)RootParamNum::Light);
	// カメラ
	cameraManager->Draw((UINT)RootParamNum::Camera);
	for (auto& object : objects) { object->Draw(); }
}

void ModelManager::Update()
{
	lightGroup->Update();
	cameraManager->Update();
	objects.remove_if([](std::unique_ptr<Object3d>& object) { return object->isDestroy; });
	for (auto& object : objects) { object->Update(); }
}