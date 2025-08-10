#include "Framework.h"
#include "Sprite.h"
#include "D3D12Common.h"
#include "ImGuiManager.h"
#include "PipelineManager.h"
using namespace WE;
using namespace _2D;

void Framework::Initialize()
{
	wAPI->Initialize(windowName);
	dxCommon->Initialize();
	input->Initialize();
	PipelineManager::Initialize();
	constant->LoadConstants();
	ImGuiManager::Initialize();
	// グローバル変数の読み込み
	globalVariables->LoadFiles();
}

void Framework::Update()
{
	// 入力関連の毎フレーム処理
	input->Update();
	// グローバル変数の更新
	globalVariables->Update();
	// ゲームシーンの毎フレーム処理
	sceneManager->Update();
	// スプライトの更新
	spMan->UpdateAll();
}

void Framework::Finalize()
{
	ImGuiManager::Finalize();
	constant->Finalize();
	sceneManager->Finalize();
	wAPI->Finalize();
}

void Framework::Run()
{
	Initialize();

	// Xボタンで終了メッセージが来たらゲームループを抜ける
	while (!(IsEndRequest() || wAPI->ProcessMessage()))
	{
		// ImGui受付開始
		ImGuiManager::Begin();

		Update();

		// ImGui受付終了
		ImGuiManager::End();

		// デスクリプタヒープをセット
		dxCommon->SetDescriptorHeap();

		Draw();
	}

	Finalize();
}