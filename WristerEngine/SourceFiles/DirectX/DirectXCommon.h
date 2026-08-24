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
		D3D12_RECT* scissorRect;
	};

	// DirectX基盤
	class DirectXCommon final
	{
	private:
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
		static const UINT FrameCount = 3;
		static const size_t MAX_SRV_COUNT = 512;

		ComPtr<ID3D12Device> device;
		ComPtr<IDXGIFactory7> dxgiFactory;
		std::array<ComPtr<ID3D12CommandAllocator>, FrameCount> commandAllocators;
		ComPtr<ID3D12GraphicsCommandList> commandList;
		ComPtr<ID3D12CommandQueue> commandQueue;
		ComPtr<IDXGISwapChain4> swapchain;
		ComPtr<ID3D12DescriptorHeap> rtvHeap, srvHeap, dsvHeap;
		ComPtr<ID3D12Resource> depthBuffer;
		UINT32 srvIndex = 0;
		ComPtr<ID3D12Fence> fence;
		DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};
		std::vector<ComPtr<ID3D12Resource>> backBuffers;
		std::array<UINT64, FrameCount> fenceValues{};
		UINT64 nextFenceValue = 1;
		D3D12_VIEWPORT viewport{};
		D3D12_RECT scissorRect{};
		FPS* fixFPS = FPS::GetInstance();
		bool isChanedResolution = false;
		UINT frameIndex = 0;

		DirectXCommon() = default;
		~DirectXCommon() = default;
		DirectXCommon(const DirectXCommon&) = delete;
		DirectXCommon& operator=(const DirectXCommon&) = delete;

		// デバイスの生成
		void CreateDevice();
		// コマンドリストの生成
		void CreateCommandList();
		// スワップチェーンの生成
		void CreateSwapchain();
		// デスクリプタヒープの生成
		void CreateDescriptorHeaps();
		// レンダーターゲットビューの生成
		void CreateRenderTargetView();
		// シェーダーリソースビューの生成
		void CreateShaderResourceView();
		D3D12_SHADER_RESOURCE_VIEW_DESC CreateSRVDesc(const D3D12_RESOURCE_DESC* texResDesc);
		// フェンスの生成
		void CreateFence();
		// GPU待機
		void WaitForGPU();
		// フレーム待機
		void WaitForFrame();

	public:
		// インスタンス取得
		static DirectXCommon* GetInstance();
		// 初期化
		void Initialize();
		// フレーム開始処理
		void BeginFrame();
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

		// SRVを上書き
		void OverwriteSRV(ID3D12Resource* resBuff, D3D12_CPU_DESCRIPTOR_HANDLE srvHandle, const D3D12_RESOURCE_DESC* texResDesc = nullptr);

		// SRVIndexをインクリメント
		void IncrementSRVIndex() { srvIndex++; }

		// ウィンドウサイズ変更
		void ChangeResolution(CR<Vector2> windowSize);

		// setter
		void SetViewport(Vector2 viewportSize = WIN_SIZE, Vector2 viewportLeftTop = {});

		// getter
		ID3D12Device* GetDevice() const { return device.Get(); }
		ID3D12GraphicsCommandList* GetCommandList() const { return commandList.Get(); }
		size_t GetBackBufferCount() const { return backBuffers.size(); }
		Matrix4 GetViewportMatrix() const;
		ID3D12DescriptorHeap* GetSRV() const { return srvHeap.Get(); }
		SRVHandle GetNextSRVHandle() const;
		bool IsChangedResolution() const { return isChanedResolution; }
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