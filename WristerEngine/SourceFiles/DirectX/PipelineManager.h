#pragma once
#include "DirectXCommon.h"
#include <map>

namespace WristerEngine
{
	enum class RootParamType { CBV, DescriptorTable };

	// パイプライン生成の設定
	struct PipelineProp
	{
	private:
		struct InputLayoutProp
		{
			LPCSTR semanticName;
			DXGI_FORMAT format;
		};

		struct BlendProp
		{
			D3D12_BLEND_OP blendOp = D3D12_BLEND_OP_ADD;
			D3D12_BLEND srcBlend = D3D12_BLEND_SRC_ALPHA;
			D3D12_BLEND destBlend = D3D12_BLEND_INV_SRC_ALPHA;
		};

		struct RootParameterProp
		{
			UINT descriptorNum;
			UINT constBuffNum;
		};

	public:
		// VS,PS,GS
		std::array<std::wstring, 3> shaderNames;
		std::vector<InputLayoutProp> inputLayoutProps;
		BlendProp blendProp;
		D3D12_PRIMITIVE_TOPOLOGY_TYPE primitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		bool isDepthTest = false; // 深度テストを行う = 3D用 か
		D3D12_DEPTH_WRITE_MASK depthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		RootParameterProp rootParamProp{};
		D3D12_TEXTURE_ADDRESS_MODE textureAddressMode = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		D3D12_CULL_MODE cullMode = D3D12_CULL_MODE_BACK;
	};

	enum class PipelineType
	{
		Sprite,
		PostEffect,
		MultiTexture,
		Primitive,
		Object,
		Dissolve,
		Particle,
		Fbx
	};

	// パイプライン管理クラス
	class PipelineManager : public DXCommonGetter
	{
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
		static std::map<PipelineType, PipelineManager> pipelines;
		ComPtr<ID3D12RootSignature> rootSignature;
		ComPtr<ID3D12PipelineState> pipelineState;

	public:
		/// <summary>
		/// パイプライン生成
		/// </summary>
		/// <param name="pipelineProp">パイプライン生成プロパティ</param>
		void CreatePipeline(const PipelineProp& pipelineProp);
		// 初期化
		static void Initialize();
		// パイプラインのセット
		static void SetPipeline(PipelineType pipelineType);
	};
}