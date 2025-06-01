#include "Framework.h"
#include "Sprite.h"
#include "D3D12Common.h"
#include "ImGuiManager.h"
#include "PipelineManager.h"
#include <GlobalVariables.h>
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
	GlobalVariables::GetInstance()->LoadFiles();
}

void Framework::Update()
{
	// ImGui受付開始
	ImGuiManager::Begin();
	// 入力関連の毎フレーム処理
	input->Update();
	// グローバル変数の更新
	GlobalVariables::GetInstance()->Update();
	// ゲームシーンの毎フレーム処理
	sceneManager->Update();
	// スプライトの更新
	Sprite::UpdateAll();
	// ImGui受付終了
	ImGuiManager::End();
}

void Framework::Finalize()
{
	ImGuiManager::Finalize();
	constant->Finalize();
	sceneManager->Finalize();
	wAPI->Finalize();
}

bool Framework::IsEndRequest()
{
	// Xボタンで終了メッセージが来たらゲームループを抜ける
	return wAPI->ProcessMessage();
}

void Framework::Run()
{
	Initialize();

	while (!IsEndRequest())
	{
		Update();

		// デスクリプタヒープをセット
		dxCommon->SetDescriptorHeap();

		Draw();
	}

	Finalize();
}