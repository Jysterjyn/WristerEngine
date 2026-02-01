#include "Framework.h"
#include "Sprite.h"
#include "D3D12Common.h"
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
	imGuiManager->Initialize();
	spMan->Initialize();
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
	spMan->Update();
	// 音源の更新
	audioManager->Update();
}

void Framework::Finalize()
{
	imGuiManager->Finalize();
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
		imGuiManager->Begin();

		Update();

		// ImGui受付終了
		imGuiManager->End();

		// デスクリプタヒープをセット
		dxCommon->SetDescriptorHeap();

		Draw();
	}

	Finalize();
}