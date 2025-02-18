#pragma once
#include <d3dx12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <vector>
#include "FPS.h"
#include "WindowsAPI.h"
#include "Matrix4.h"
#include <DirectXTex.h>

namespace WristerEngine
{
	struct SRVHandle
	{
		D3D12_CPU_DESCRIPTOR_HANDLE cpu{};
		D3D12_GPU_DESCRIPTOR_HANDLE	gpu{};
	};

	// DirectX基盤
	class DirectXCommon final
	{
	private:
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

		static const size_t MAX_SRV_COUNT = 512;
		ComPtr<ID3D12Device> device;
		ComPtr<IDXGIFactory7> dxgiFactory;
		ComPtr<ID3D12CommandAllocator> commandAllocator;
		ComPtr<ID3D12GraphicsCommandList> commandList;
		ComPtr<ID3D12CommandQueue> commandQueue;
		ComPtr<IDXGISwapChain4> swapchain;
		ComPtr<ID3D12DescriptorHeap> dsvHeap, srvHeap, rtvHeap;
		UINT32 srvIndex = 0;
		ComPtr<ID3D12Fence> fence;
		DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};
		std::vector<ComPtr<ID3D12Resource>> backBuffers;
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle{};
		UINT64 fenceVal = 0;
		D3D12_VIEWPORT viewport{};
		FPS* fixFPS = FPS::GetInstance();

		DirectXCommon() = default;
		~DirectXCommon() = default;
		// デバイスの初期化
		void InitializeDevice();
		// コマンドリストの初期化
		void InitializeCommand();
		// スワップチェーンの初期化
		void InitializeSwapchain();
		// レンダーターゲットビューの初期化
		void InitializeRenderTargetView();
		// シェーダーリソースビューの初期化
		void InitializeShaderResourceView();
		// 深度バッファの初期化
		void InitializeDepthBuffer();
		// フェンスの初期化
		void InitializeFence();

	public:
		DirectXCommon(const DirectXCommon& obj) = delete;
		DirectXCommon& operator=(const DirectXCommon& obj) = delete;

		// インスタンス取得
		static DirectXCommon* GetInstance();
		// 初期化
		void Initialize();
		// 描画前処理
		void PreDraw();
		// 描画後処理
		void PostDraw();

		/// <summary>
		/// SRVを生成
		/// </summary>
		/// <returns>現在生成したSRVハンドル</returns>
		SRVHandle CreateSRV(ID3D12Resource* resBuff, uint32_t mipLevels = UINT32_MAX, const D3D12_RESOURCE_DESC* texResDesc = nullptr);

		// setter
		void SetViewport(Vector2 viewportSize = WIN_SIZE, Vector2 viewportLeftTop = {});
		// getter
		ID3D12Device* GetDevice() const { return device.Get(); }
		ID3D12GraphicsCommandList* GetCommandList() const { return commandList.Get(); }
		size_t GetBackBufferCount() const { return backBuffers.size(); }
		Matrix4 GetViewportMatrix() const;
		ID3D12DescriptorHeap* GetSRV() const { return srvHeap.Get(); }
	};
}