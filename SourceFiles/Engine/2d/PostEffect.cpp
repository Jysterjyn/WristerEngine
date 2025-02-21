#include "PostEffect.h"
#include "D3D12Common.h"

using namespace WristerEngine;
using namespace WristerEngine::_2D;

ID3D12Device* PostEffect::device;
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

	// パイプラインステートとルートシグネチャの設定コマンド
	PipelineManager::SetPipeline(PipelineType::PostEffect);
	// プリミティブ形状の設定コマンド
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); // 三角形リスト

	cmdList->SetGraphicsRootDescriptorTable(0, srvHandle.gpu);

	// 頂点バッファビューの設定コマンド
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	cmdList->SetGraphicsRootConstantBufferView(1, constBuff->GetGPUVirtualAddress());
	// 描画コマンド
	cmdList->DrawInstanced((UINT)vertices.size(), 1, 0, 0); // 全ての頂点を使って描画
}

void PostEffect::PreDrawScene()
{
	// ビューポート領域の設定
	D3D12_VIEWPORT viewport= CD3DX12_VIEWPORT(0.0f, 0.0f, WIN_SIZE.x, WIN_SIZE.y);

	DirectXCommon::GetInstance()->PreDraw({ texBuff.Get(),
		rtvHeap.Get(), dsvHeap.Get(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, 0, &viewport});


	//ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();

	//// リソースバリアで書き込み可能に変更
	//D3D12_RESOURCE_BARRIER resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
	//	texBuff.Get(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
	//cmdList->ResourceBarrier(1, &resourceBarrier);

	//// 描画先のRTVとDSVを指定する
	//D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
	//D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();
	//cmdList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

	//// 画面全体の色をクリア
	//float clearColor[4] = { 0,0,0,1 };
	//cmdList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	//// 画面全体の深度をクリア
	//cmdList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	//// SRV用のデスクリプタヒープを指定する
	//ID3D12DescriptorHeap* ppHeaps[] = { DirectXCommon::GetInstance()->GetSRV() };
	//cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//// ビューポート領域の設定
	//CD3DX12_VIEWPORT viewport(0.0f, 0.0f, WIN_SIZE.x, WIN_SIZE.y);
	//cmdList->RSSetViewports(1, &viewport);
	//// シザー矩形の設定
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