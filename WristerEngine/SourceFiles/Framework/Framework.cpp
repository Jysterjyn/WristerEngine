#include "Framework.h"
#include "Sprite.h"
#include "D3D12Common.h"
#include "ImGuiManager.h"
#include "PipelineManager.h"
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
	Sprite::UpdateAll();
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
	return wAPI->ProcessMessage();
}

void Framework::Run()
{
	Initialize();

	while (!IsEndRequest())
	{
		Update();

		// �f�X�N���v�^�q�[�v���Z�b�g
		//dxCommon->SetDescriptorHeap();

		Draw();
	}

	Finalize();
}