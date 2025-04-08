#pragma once
#include "DirectXCommon.h"
#include <map>

namespace WristerEngine
{
	enum class RootParamType { CBV, DescriptorTable };

	// �p�C�v���C�������̐ݒ�
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
		bool isDepthTest = false; // �[�x�e�X�g���s�� = 3D�p ��
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

	// �p�C�v���C���Ǘ��N���X
	class PipelineManager : public DXCommonGetter
	{
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
		static std::map<PipelineType, PipelineManager> pipelines;
		ComPtr<ID3D12RootSignature> rootSignature;
		ComPtr<ID3D12PipelineState> pipelineState;

	public:
		/// <summary>
		/// �p�C�v���C������
		/// </summary>
		/// <param name="pipelineProp">�p�C�v���C�������v���p�e�B</param>
		void CreatePipeline(const PipelineProp& pipelineProp);
		// ������
		static void Initialize();
		// �p�C�v���C���̃Z�b�g
		static void SetPipeline(PipelineType pipelineType);
	};
}