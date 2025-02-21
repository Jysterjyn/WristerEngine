#include "PostEffect.h"
#include "D3D12Common.h"

using namespace WristerEngine;
using namespace WristerEngine::_2D;

ID3D12Device* PostEffect::device;
std::vector<std::unique_ptr<PostEffect>> PostEffect::postEffects;

#pragma region �����֐�
void PostEffect::CreateBuffers()
{
	vertices =
	{ {
		{ { -1, -1 }, { 0, 1 } },
		{ { -1, +1 }, { 0, 0 } },
		{ { +1, -1 }, { 1, 1 } },
		{ { +1, +1 }, { 1, 0 } }
	} };

	Vertex* vertMap = nullptr;
	ID3D12Resource* vertBuff = nullptr;
	CreateBuffer<Vertex>(&vertBuff, &vertMap, sizeof(Vertex) * vertices.size());

	// �S���_�ɑ΂��č��W���R�s�[
	copy(vertices.begin(), vertices.end(), vertMap);

	// GPU���z�A�h���X
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	vbView.SizeInBytes = sizeof(Vertex) * (UINT)vertices.size();
	// ���_1���̃f�[�^�T�C�Y
	vbView.StrideInBytes = sizeof(Vertex);

	// �萔�o�b�t�@
	CreateBuffer(constBuff.GetAddressOf(),
		&constMap, (sizeof(ConstBufferData) + 0xff) & ~0xff);

	Result result;

	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, (UINT64)WIN_SIZE.x, (UINT)WIN_SIZE.y,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

	float clearColor[4] = { 0,0,0,1 };
	CD3DX12_CLEAR_VALUE clearValue(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, clearColor);

	result = device->CreateCommittedResource(
		new CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&clearValue,
		IID_PPV_ARGS(&texBuff));

	const UINT PIXEL_COUNT = (UINT)WIN_SIZE.x * (UINT)WIN_SIZE.y;
	const UINT ROW_PITCH = sizeof(UINT) * (UINT)WIN_SIZE.x;
	const UINT DEPTH_PITCH = ROW_PITCH * (UINT)WIN_SIZE.y;
	UINT* img = new UINT[PIXEL_COUNT];
	for (size_t i = 0; i < PIXEL_COUNT; i++) { img[i] = 0xff0000ff; }

	result = texBuff->WriteToSubresource(0, nullptr, img, ROW_PITCH, DEPTH_PITCH);
	delete[] img;
}

void PostEffect::CreateRTV()
{
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.NumDescriptors = 1;
	Result result = device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap));

	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	device->CreateRenderTargetView(texBuff.Get(), &rtvDesc, rtvHeap->GetCPUDescriptorHandleForHeapStart());
}
#pragma endregion

PostEffect* WristerEngine::_2D::PostEffect::Create(Type effectType)
{
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	if (!device) { device = dxCommon->GetDevice(); }

	std::unique_ptr<PostEffect> postEffect = std::make_unique<PostEffect>();
	postEffect->CreateBuffers();
	postEffect->CreateRTV();
	postEffect->srvHandle = dxCommon->CreateSRV(postEffect->texBuff.Get());
	dxCommon->InitializeDepthBuffer(&postEffect->dsvHeap);
	postEffect->SetEffectType(effectType);
	postEffects.push_back(std::move(postEffect));
	return postEffects.back().get();
}

void PostEffect::Draw()
{
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();

	// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	PipelineManager::SetPipeline(PipelineType::PostEffect);
	// �v���~�e�B�u�`��̐ݒ�R�}���h
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); // �O�p�`���X�g

	cmdList->SetGraphicsRootDescriptorTable(0, srvHandle.gpu);

	// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	cmdList->SetGraphicsRootConstantBufferView(1, constBuff->GetGPUVirtualAddress());
	// �`��R�}���h
	cmdList->DrawInstanced((UINT)vertices.size(), 1, 0, 0); // �S�Ă̒��_���g���ĕ`��
}

void PostEffect::PreDrawScene()
{
	// �r���[�|�[�g�̈�̐ݒ�
	D3D12_VIEWPORT viewport= CD3DX12_VIEWPORT(0.0f, 0.0f, WIN_SIZE.x, WIN_SIZE.y);

	DirectXCommon::GetInstance()->PreDraw({ texBuff.Get(),
		rtvHeap.Get(), dsvHeap.Get(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, 0, &viewport});


	//ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();

	//// ���\�[�X�o���A�ŏ������݉\�ɕύX
	//D3D12_RESOURCE_BARRIER resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
	//	texBuff.Get(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
	//cmdList->ResourceBarrier(1, &resourceBarrier);

	//// �`����RTV��DSV���w�肷��
	//D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
	//D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();
	//cmdList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

	//// ��ʑS�̂̐F���N���A
	//float clearColor[4] = { 0,0,0,1 };
	//cmdList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	//// ��ʑS�̂̐[�x���N���A
	//cmdList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	//// SRV�p�̃f�X�N���v�^�q�[�v���w�肷��
	//ID3D12DescriptorHeap* ppHeaps[] = { DirectXCommon::GetInstance()->GetSRV() };
	//cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//// �r���[�|�[�g�̈�̐ݒ�
	//CD3DX12_VIEWPORT viewport(0.0f, 0.0f, WIN_SIZE.x, WIN_SIZE.y);
	//cmdList->RSSetViewports(1, &viewport);
	//// �V�U�[��`�̐ݒ�
	//CD3DX12_RECT rect(0, 0, (LONG)WIN_SIZE.x, (LONG)WIN_SIZE.y);
	//cmdList->RSSetScissorRects(1, &rect);
}

void PostEffect::PostDrawScene()
{
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();

	CD3DX12_RESOURCE_BARRIER resBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
		texBuff.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	cmdList->ResourceBarrier(1, &resBarrier);
}