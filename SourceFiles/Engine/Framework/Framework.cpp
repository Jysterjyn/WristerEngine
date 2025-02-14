#include "Framework.h"
#include "Sprite.h"
#include "D3D12Common.h"
#include "ImGuiManager.h"
using namespace WristerEngine;

void Framework::Initialize()
{
	wAPI->Initialize(windowName);
	dxCommon->Initialize();
	input->Initialize();
	PipelineManager::Initialize();
	constant->LoadConstants();
	_2D::ImGuiManager::Initialize();
}

void Framework::Update()
{
	_2D::ImGuiManager::Begin();
	input->Update();
	sceneManager->Update();
	_2D::ImGuiManager::End();
}

void Framework::Finalize()
{
	_2D::ImGuiManager::Finalize();
	constant->Finalize();
	sceneManager->Finalize();
	wAPI->Finalize();
}

bool Framework::IsEndRequest()
{
	// Xボタンで終了メッセージが来たらゲームループを抜ける
	if (wAPI->ProcessMessage()) { return true; }
	// ESCキーを押したらゲームループを抜ける
	if (input->IsTrigger(WristerEngine::Key::Escape)) { return true; }
	return false;
}

void Framework::Run()
{
	Initialize();

	while (!IsEndRequest())
	{
		Update();
		Draw();
	}

	Finalize();
}