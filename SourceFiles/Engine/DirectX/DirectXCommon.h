#pragma once
#include <d3dx12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include "FPS.h"
#include "WindowsAPI.h"
#include "Matrix4.h"
#include <DirectXTex.h>

namespace WristerEngine
{
	namespace _2D
	{
		struct Vertex { Vector2 pos, uv; };
	}

	struct SRVHandle
	{
		D3D12_CPU_DESCRIPTOR_HANDLE cpu{};
		D3D12_GPU_DESCRIPTOR_HANDLE	gpu{};
	};

	struct PreDrawProp
	{
		ID3D12Resource* resBuff;
		ID3D12DescriptorHeap* rtvHeap, * dsvHeap;
		D3D12_RESOURCE_STATES state;
		UINT rtvIndex;
		D3D12_VIEWPORT* viewport;
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
		ComPtr<ID3D12DescriptorHeap> rtvHeap, srvHeap, dsvHeap;
		UINT32 srvIndex = 0;
		ComPtr<ID3D12Fence> fence;
		DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};
		std::vector<ComPtr<ID3D12Resource>> backBuffers;
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
		// フェンスの初期化
		void InitializeFence();

	public:
		DirectXCommon(const DirectXCommon& obj) = delete;
		DirectXCommon& operator=(const DirectXCommon& obj) = delete;

		// インスタンス取得
		static DirectXCommon* GetInstance();
		// 初期化
		void Initialize();
		// 深度バッファの初期化
		void InitializeDepthBuffer(ID3D12DescriptorHeap** dsvHeap) const;
		// 描画前処理
		void PreDraw();
		// ポストエフェクトクラス内で使う共通処理部分
		void PreDraw(const PreDrawProp& prop);
		// 描画後処理
		void PostDraw();

		/// <summary>
		/// SRVを生成
		/// </summary>
		/// <returns>現在生成したSRVハンドル</returns>
		SRVHandle CreateSRV(ID3D12Resource* resBuff, const D3D12_RESOURCE_DESC* texResDesc = nullptr);

		// SRVIndexをインクリメント
		void IncrementSRVIndex() { srvIndex++; }

		// setter
		void SetViewport(Vector2 viewportSize = WIN_SIZE, Vector2 viewportLeftTop = {});

		// getter
		ID3D12Device* GetDevice() const { return device.Get(); }
		ID3D12GraphicsCommandList* GetCommandList() const { return commandList.Get(); }
		size_t GetBackBufferCount() const { return backBuffers.size(); }
		Matrix4 GetViewportMatrix() const;
		ID3D12DescriptorHeap* GetSRV() const { return srvHeap.Get(); }
		SRVHandle GetNextSRVHandle() const;
	};

	struct DXCommonGetter
	{
	protected:
		static ID3D12Device* device;
		static DirectXCommon* dxCommon;
		static ID3D12GraphicsCommandList* cmdList;

	public:
		static void SetPointer(ID3D12Device* device_, ID3D12GraphicsCommandList* cmdList_)
		{
			device = device_;
			cmdList = cmdList_;
		}
	};
}