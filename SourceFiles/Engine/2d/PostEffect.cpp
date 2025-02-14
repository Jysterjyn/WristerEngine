#include "PostEffect.h"
#include "D3D12Common.h"

using namespace WristerEngine;
using namespace WristerEngine::_2D;

const float PostEffect::CLEAR_COLOR[4] = { 0,0,0,1 };
Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> PostEffect::descHeapSRV;
Microsoft::WRL::ComPtr<ID3D12RootSignature> PostEffect::rootSignature;
Microsoft::WRL::ComPtr<ID3D12PipelineState> PostEffect::pipelineState;
ID3D12Device* PostEffect::device;
int PostEffect::staticSRVIndex = 0;
std::vector<std::unique_ptr<PostEffect>> PostEffect::postEffects;

#pragma region 生成関数
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

	// 全頂点に対して座標をコピー
	copy(vertices.begin(), vertices.end(), vertMap);

	// GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vbView.SizeInBytes = sizeof(Vertex) * (UINT)vertices.size();
	// 頂点1つ分のデータサイズ
	vbView.StrideInBytes = sizeof(Vertex);

	// 定数バッファ
	CreateBuffer(constBuff.GetAddressOf(),
		&constMap, (sizeof(ConstBufferData) + 0xff) & ~0xff);

	Result result;

	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, (UINT64)WIN_SIZE.x, (UINT)WIN_SIZE.y,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

	CD3DX12_CLEAR_VALUE clearValue(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, CLEAR_COLOR);

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

void PostEffect::CreateSRV()
{
	srvIndex = staticSRVIndex++;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	device->CreateShaderResourceView(texBuff.Get(), &srvDesc,
		CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeapSRV->GetCPUDescriptorHandleForHeapStart(), srvIndex,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
}

void PostEffect::CreateRTV()
{
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.NumDescriptors = 1;
	Result result = device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&descHeapRTV));

	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	device->CreateRenderTargetView(texBuff.Get(), &rtvDesc, descHeapRTV->GetCPUDescriptorHandleForHeapStart());
}

void PostEffect::CreateDSV()
{
	CD3DX12_RESOURCE_DESC depthResourceDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_D32_FLOAT,
			(UINT64)WIN_SIZE.x, (UINT)WIN_SIZE.y,
			1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	CD3DX12_CLEAR_VALUE clearValue(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);

	Result result = device->CreateCommittedResource(
		new CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE, &depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&clearValue,
		IID_PPV_ARGS(&depthBuff));

	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.NumDescriptors = 1;
	result = device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&descHeapDSV));

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(depthBuff.Get(), &dsvDesc, descHeapDSV->GetCPUDescriptorHandleForHeapStart());
}
#pragma endregion

void PostEffect::StaticInitialize()
{
	device = DirectXCommon::GetInstance()->GetDevice();

	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc{};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = 16;

	Result result = device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&descHeapSRV));
}

PostEffect* WristerEngine::_2D::PostEffect::Create(Type effectType)
{
	std::unique_ptr<PostEffect> postEffect = std::make_unique<PostEffect>();
	postEffect->CreateBuffers();
	postEffect->CreateSRV();
	postEffect->CreateRTV();
	postEffect->CreateDSV();
	postEffect->SetEffectType(effectType);
	postEffects.push_back(std::move(postEffect));
	return postEffects.back().get();
}

void PostEffect::Draw()
{
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();

	// パイプラインステートとルートシグネチャの設定コマンド
	PipelineManager::SetPipeline(PipelineType::PostEffect);
	// プリミティブ形状の設定コマンド
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); // 三角形リスト
	// デスクリプタヒープの設定コマンド
	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	cmdList->SetGraphicsRootDescriptorTable(0, GetGPUHandle());

	// 頂点バッファビューの設定コマンド
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	cmdList->SetGraphicsRootConstantBufferView(1, constBuff->GetGPUVirtualAddress());
	// 描画コマンド
	cmdList->DrawInstanced((UINT)vertices.size(), 1, 0, 0); // 全ての頂点を使って描画
}

void PostEffect::PreDrawScene()
{
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();

	CD3DX12_RESOURCE_BARRIER resBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
		texBuff.Get(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);

	cmdList->ResourceBarrier(1, &resBarrier);

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = descHeapRTV->GetCPUDescriptorHandleForHeapStart();
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = descHeapDSV->GetCPUDescriptorHandleForHeapStart();
	cmdList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

	CD3DX12_VIEWPORT viewport(0.0f, 0.0f, WIN_SIZE.x, WIN_SIZE.y);
	CD3DX12_RECT rect(0, 0, (LONG)WIN_SIZE.x, (LONG)WIN_SIZE.y);
	// ビューポート設定コマンドを、コマンドリストに積む
	cmdList->RSSetViewports(1, &viewport);
	cmdList->RSSetScissorRects(1, &rect); // シザー矩形設定コマンドを、コマンドリストに積む

	cmdList->ClearRenderTargetView(rtvHandle, CLEAR_COLOR, 0, nullptr);
	cmdList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

D3D12_GPU_DESCRIPTOR_HANDLE PostEffect::GetGPUHandle() const
{
	return CD3DX12_GPU_DESCRIPTOR_HANDLE(
		descHeapSRV->GetGPUDescriptorHandleForHeapStart(), srvIndex,
		device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
}

void PostEffect::PostDrawScene()
{
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
	
	CD3DX12_RESOURCE_BARRIER resBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
		texBuff.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	
	cmdList->ResourceBarrier(1, &resBarrier);
}