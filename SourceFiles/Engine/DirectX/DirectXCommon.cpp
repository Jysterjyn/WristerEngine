#include "D3D12Common.h"
#include <thread>
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
using namespace std;
using namespace WristerEngine;
using namespace Microsoft::WRL;

ID3D12Device* DXCommonGetter::device = nullptr;
DirectXCommon* DXCommonGetter::dxCommon = DirectXCommon::GetInstance();
ID3D12GraphicsCommandList* DXCommonGetter::cmdList = nullptr;

DirectXCommon* DirectXCommon::GetInstance()
{
	static DirectXCommon* dxCommon = new DirectXCommon;
	return dxCommon;
}

void DirectXCommon::Initialize()
{
	fixFPS->Initialize();				// FPS固定初期化
	InitializeDevice();					// デバイスの生成
	InitializeCommand();				// コマンド関連の初期化
	InitializeSwapchain();				// スワップチェーンの初期化
	InitializeRenderTargetView();		// レンダーターゲットビューの初期化
	InitializeShaderResourceView();		// シェーダーリソースビューの初期化
	InitializeDepthBuffer(&dsvHeap);	// 深度バッファの初期化
	InitializeFence();					// フェンスの初期化

	// DXCommonGetterにポインタ代入
	DXCommonGetter::SetPointer(device.Get(), commandList.Get());

	// ビューポート設定コマンド
	viewport = CD3DX12_VIEWPORT(0.0f, 0.0f, WIN_SIZE.x, WIN_SIZE.y);
}

void DirectXCommon::InitializeDevice()
{
#ifdef _DEBUG
	//デバッグレイヤーをオンに
	ComPtr<ID3D12Debug1> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		debugController->EnableDebugLayer();
		debugController->SetEnableGPUBasedValidation(TRUE);
	}
#endif
	Result result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));

	vector<ComPtr<IDXGIAdapter4>> adapters;
	ComPtr<IDXGIAdapter4> tmpAdapter;
	// パフォーマンスが高いものから順に、全てのアダプターを列挙する
	for (UINT i = 0;
		dxgiFactory->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
			IID_PPV_ARGS(&tmpAdapter)) != DXGI_ERROR_NOT_FOUND;	i++)
	{
		// 動的配列に追加する
		adapters.push_back(tmpAdapter);
	}
	// 妥当なアダプタを選別する
	for (auto& adapter : adapters)
	{
		DXGI_ADAPTER_DESC3 adapterDesc;
		// アダプターの情報を取得する
		adapter->GetDesc3(&adapterDesc);
		// ソフトウェアデバイスを回避
		if ((adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) { continue; }
		// デバイスを採用してループを抜ける
		tmpAdapter = adapter;
		break;
	}

	D3D_FEATURE_LEVEL featureLevel{};
	// 対応レベルの配列
	vector<D3D_FEATURE_LEVEL> levels =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	for (auto& level : levels)
	{
		// 採用したアダプターでデバイスを生成
		if (FAILED(D3D12CreateDevice(tmpAdapter.Get(), level, IID_PPV_ARGS(&device)))) { continue; }
		// デバイスを生成できた時点でループを抜ける
		featureLevel = level;
		break;
	}

#ifdef _DEBUG
	// エラー時にブレークを発生させる
	ComPtr<ID3D12InfoQueue> infoQueue;
	if (SUCCEEDED(device->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
		// 抑制するエラー
		D3D12_MESSAGE_ID denyIds[] = {
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE };
		// 抑制する表示レベル
		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter{};
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;
		// 指定したエラーの表示を抑制する
		infoQueue->PushStorageFilter(&filter);
		// エラー時にブレークを発生させる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
	}
#endif
}

void DirectXCommon::InitializeCommand()
{
	Result result = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));

	result = device->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		commandAllocator.Get(), nullptr,
		IID_PPV_ARGS(&commandList));

	device->CreateCommandQueue(new D3D12_COMMAND_QUEUE_DESC(), IID_PPV_ARGS(&commandQueue));
}

void DirectXCommon::InitializeSwapchain()
{
	swapchainDesc.Width = (UINT)WIN_SIZE.x;
	swapchainDesc.Height = (UINT)WIN_SIZE.y;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 色情報の書式
	swapchainDesc.SampleDesc.Count = 1; // マルチサンプルしない
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER; // バックバッファ用
	swapchainDesc.BufferCount = 2; // バッファ数を2つに設定
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // フリップ後は破棄
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	ComPtr<IDXGISwapChain1> swapchain1;
	Result result = dxgiFactory->CreateSwapChainForHwnd(
		commandQueue.Get(), WindowsAPI::GetInstance()->GetHwnd(), &swapchainDesc, nullptr, nullptr,
		&swapchain1);

	result = swapchain1->QueryInterface(IID_PPV_ARGS(&swapchain));
}

void DirectXCommon::InitializeRenderTargetView()
{
	// デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // レンダーターゲットビュー
	rtvHeapDesc.NumDescriptors = swapchainDesc.BufferCount; // 裏表の2つ
	// デスクリプタヒープの生成
	Result result = device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap));

	backBuffers.resize(swapchainDesc.BufferCount);

	for (int i = 0; i < backBuffers.size(); i++)
	{
		swapchain->GetBuffer((UINT)i, IID_PPV_ARGS(&backBuffers[i]));
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(rtvHeap->GetCPUDescriptorHandleForHeapStart(),
			i, device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		device->CreateRenderTargetView(backBuffers[i].Get(), &rtvDesc, rtvHandle);
	}
}

void WristerEngine::DirectXCommon::InitializeShaderResourceView()
{
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc{};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = MAX_SRV_COUNT;
	Result result = device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
}

void DirectXCommon::InitializeDepthBuffer(ID3D12DescriptorHeap** dsvHeap_) const
{
	D3D12_RESOURCE_DESC depthResourceDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_D32_FLOAT,
			(UINT64)WIN_SIZE.x, (UINT)WIN_SIZE.y,
			1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	ID3D12Resource* depthBuff = nullptr;
	Result result = device->CreateCommittedResource(
		new CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		new CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&depthBuff));

	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	result = device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(dsvHeap_));

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(
		depthBuff, &dsvDesc, (*dsvHeap_)->GetCPUDescriptorHandleForHeapStart());
}

void DirectXCommon::InitializeFence()
{
	Result result = device->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
}

void DirectXCommon::PreDraw()
{
	UINT bbIndex = swapchain->GetCurrentBackBufferIndex();

	PreDraw({ backBuffers[bbIndex].Get(), rtvHeap.Get(),
		dsvHeap.Get(), D3D12_RESOURCE_STATE_PRESENT, bbIndex, &viewport });
}

void WristerEngine::DirectXCommon::PreDraw(const PreDrawProp& prop)
{
	// リソースバリアで書き込み可能に変更
	D3D12_RESOURCE_BARRIER resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
		prop.resBuff, prop.state, D3D12_RESOURCE_STATE_RENDER_TARGET);
	commandList->ResourceBarrier(1, &resourceBarrier);

	// 描画先のRTVとDSVを指定する
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(prop.rtvHeap->GetCPUDescriptorHandleForHeapStart(),
		prop.rtvIndex, device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));

	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = prop.dsvHeap->GetCPUDescriptorHandleForHeapStart();
	commandList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

	// 画面全体の色をクリア
	float clearColor[4] = { 0,0,0,1 };
	commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	// 画面全体の深度をクリア
	commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// SRV用のデスクリプタヒープを指定する
	ID3D12DescriptorHeap* ppHeaps[] = { srvHeap.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// ビューポート領域の設定
	commandList->RSSetViewports(1, prop.viewport);
	// シザー矩形の設定
	commandList->RSSetScissorRects(1, std::make_unique<CD3DX12_RECT>(0, 0, (LONG)WIN_SIZE.x, (LONG)WIN_SIZE.y).get());
}

void DirectXCommon::PostDraw()
{
	UINT bbIndex = swapchain->GetCurrentBackBufferIndex();

	D3D12_RESOURCE_BARRIER resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
		backBuffers[bbIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

	commandList->ResourceBarrier(1, &resourceBarrier);

	// 命令のクローズ
	Result result = commandList->Close();
	// コマンドリストの実行
	ID3D12CommandList* commandLists[] = { commandList.Get() };
	commandQueue->ExecuteCommandLists(1, commandLists);

	// 画面に表示するバッファをフリップ(裏表の入替え)
	result = swapchain->Present(1, 0);

	// コマンドの実行完了を待つ
	commandQueue->Signal(fence.Get(), ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		if (event != 0)
		{
			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}
	}

	fixFPS->Fix(); // FPS固定

	// キューをクリア
	result = commandAllocator->Reset();
	// 再びコマンドリストを貯める準備
	result = commandList->Reset(commandAllocator.Get(), nullptr);
}

SRVHandle DirectXCommon::CreateSRV(ID3D12Resource* resBuff, const D3D12_RESOURCE_DESC* texResDesc)
{
	// テクスチャ枚数上限チェック
	assert(srvIndex < MAX_SRV_COUNT);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	if (texResDesc)
	{
		srvDesc.Format = texResDesc->Format;
		srvDesc.Texture2D.MipLevels = texResDesc->MipLevels;
	}
	else
	{
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		srvDesc.Texture2D.MipLevels = 1;
	}
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;

	SRVHandle srvHandle = GetNextSRVHandle();
	device->CreateShaderResourceView(resBuff, &srvDesc, srvHandle.cpu);
	srvIndex++;
	return srvHandle;
}

void DirectXCommon::SetViewport(Vector2 viewportSize, Vector2 viewportLeftTop)
{
	viewport = CD3DX12_VIEWPORT(viewportLeftTop.x, viewportLeftTop.y,
		viewportSize.x, viewportSize.y);
}

Matrix4 DirectXCommon::GetViewportMatrix() const
{
	Matrix4 mat;
	mat.m[0][0] = viewport.Width / 2.0f;
	mat.m[1][1] = viewport.Height / 2.0f;
	mat.m[3][0] = viewport.TopLeftX + viewport.Width / 2.0f;
	mat.m[3][1] = viewport.TopLeftY + viewport.Height / 2.0f;
	return mat;
}

SRVHandle DirectXCommon::GetNextSRVHandle() const
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuHandle(srvHeap->GetCPUDescriptorHandleForHeapStart(),
		srvIndex, device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));

	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle(srvHeap->GetGPUDescriptorHandleForHeapStart(),
		srvIndex, device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));

	return SRVHandle(cpuHandle, gpuHandle);
}
