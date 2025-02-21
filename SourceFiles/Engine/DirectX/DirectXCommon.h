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

	// DirectX���
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
		// �f�o�C�X�̏�����
		void InitializeDevice();
		// �R�}���h���X�g�̏�����
		void InitializeCommand();
		// �X���b�v�`�F�[���̏�����
		void InitializeSwapchain();
		// �����_�[�^�[�Q�b�g�r���[�̏�����
		void InitializeRenderTargetView();
		// �V�F�[�_�[���\�[�X�r���[�̏�����
		void InitializeShaderResourceView();
		// �t�F���X�̏�����
		void InitializeFence();

	public:
		DirectXCommon(const DirectXCommon& obj) = delete;
		DirectXCommon& operator=(const DirectXCommon& obj) = delete;

		// �C���X�^���X�擾
		static DirectXCommon* GetInstance();
		// ������
		void Initialize();
		// �[�x�o�b�t�@�̏�����
		void InitializeDepthBuffer(ID3D12DescriptorHeap** dsvHeap) const;
		// �`��O����
		void PreDraw();
		// �|�X�g�G�t�F�N�g�N���X���Ŏg�����ʏ�������
		void PreDraw(const PreDrawProp& prop);
		// �`��㏈��
		void PostDraw();

		/// <summary>
		/// SRV�𐶐�
		/// </summary>
		/// <returns>���ݐ�������SRV�n���h��</returns>
		SRVHandle CreateSRV(ID3D12Resource* resBuff, const D3D12_RESOURCE_DESC* texResDesc = nullptr);

		// SRVIndex���C���N�������g
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