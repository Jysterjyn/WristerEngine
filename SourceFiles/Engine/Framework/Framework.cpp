#include "Framework.h"
#include "Sprite.h"
#include "D3D12Common.h"
#include "ImGuiManager.h"
using namespace WristerEngine;
using namespace _2D;

void Framework::Initialize()
{
	wAPI->Initialize(windowName);
	dxCommon->Initialize();
	input->Initialize();
	PipelineManager::Initialize();
	constant->LoadConstants();
	ImGuiManager::Initialize();
}

void Framework::Update()
{
	ImGuiManager::Begin();
	input->Update();
	sceneManager->Update();
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
	// X�{�^���ŏI�����b�Z�[�W��������Q�[�����[�v�𔲂���
	if (wAPI->ProcessMessage()) { return true; }
	// ESC�L�[����������Q�[�����[�v�𔲂���
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