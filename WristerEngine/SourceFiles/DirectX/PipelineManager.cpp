#include "PipelineManager.h"
#include "D3D12Common.h"
using namespace WristerEngine;

std::map<PipelineType, PipelineManager> PipelineManager::pipelines;

void PipelineManager::Initialize()
{
	PipelineProp pipelineProp;
	pipelineProp.shaderNames = { L"SpriteVS", L"SpritePS" };
	pipelineProp.inputLayoutProps.push_back({ "POSITION", DXGI_FORMAT_R32G32_FLOAT });
	pipelineProp.inputLayoutProps.push_back({ "TEXCOORD", DXGI_FORMAT_R32G32_FLOAT });
	pipelineProp.rootParamProp = { 1,1 };
	pipelineProp.cullMode = D3D12_CULL_MODE_NONE;
	pipelines[PipelineType::Sprite].CreatePipeline(pipelineProp);

	pipelineProp.shaderNames = { L"PostEffectVS", L"PostEffectPS" };
	pipelineProp.textureAddressMode = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	pipelineProp.cullMode = D3D12_CULL_MODE_BACK;
	pipelines[PipelineType::PostEffect].CreatePipeline(pipelineProp);

	pipelineProp.shaderNames = { L"PostEffectVS", L"MultiTexturePS" };
	pipelineProp.rootParamProp = { 2,1 };
	pipelines[PipelineType::MultiTexture].CreatePipeline(pipelineProp);

	pipelineProp.shaderNames = { L"PrimitiveVS", L"PrimitivePS" };
	pipelineProp.inputLayoutProps.clear();
	pipelineProp.inputLayoutProps.push_back({ "POSITION", DXGI_FORMAT_R32G32B32_FLOAT });
	pipelineProp.inputLayoutProps.push_back({ "COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT });
	pipelineProp.isDepthTest = true;
	pipelineProp.rootParamProp = { 0,1 };
	pipelineProp.primitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
	pipelines[PipelineType::Primitive].CreatePipeline(pipelineProp);

	pipelineProp.shaderNames = { L"ObjVS", L"ObjPS" };
	pipelineProp.inputLayoutProps.clear();
	pipelineProp.inputLayoutProps.push_back({ "POSITION", DXGI_FORMAT_R32G32B32_FLOAT });
	pipelineProp.inputLayoutProps.push_back({ "NORMAL", DXGI_FORMAT_R32G32B32_FLOAT });
	pipelineProp.inputLayoutProps.push_back({ "TEXCOORD", DXGI_FORMAT_R32G32_FLOAT });
	pipelineProp.textureAddressMode = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	pipelineProp.rootParamProp = { 5,4 };
	pipelineProp.primitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	pipelines[PipelineType::Object].CreatePipeline(pipelineProp);

	pipelineProp.cullMode = D3D12_CULL_MODE_NONE;
	pipelines[PipelineType::Dissolve].CreatePipeline(pipelineProp);

	// シェーダの読み込みとコンパイル
	pipelineProp.shaderNames = { L"FBXVS", L"FBXPS" };
	// 影響を受けるボーン番号(4つ)
	pipelineProp.inputLayoutProps.push_back({ "BONEINDICES", DXGI_FORMAT_R32G32B32A32_UINT });
	// ボーンのスキンウェイト(4つ)
	pipelineProp.inputLayoutProps.push_back({ "BONEWEIGHTS", DXGI_FORMAT_R32G32B32A32_FLOAT });
	pipelineProp.rootParamProp = { 3,4 };
	pipelineProp.cullMode = D3D12_CULL_MODE_BACK;
	// グラフィックスパイプラインの生成
	pipelines[PipelineType::Fbx].CreatePipeline(pipelineProp);

	pipelineProp.shaderNames = { L"ParticleVS", L"ParticlePS", L"ParticleGS" };
	pipelineProp.inputLayoutProps.clear();
	pipelineProp.inputLayoutProps.push_back({ "POSITION", DXGI_FORMAT_R32G32B32_FLOAT });
	pipelineProp.inputLayoutProps.push_back({ "VELOCITY", DXGI_FORMAT_R32G32B32_FLOAT });
	pipelineProp.inputLayoutProps.push_back({ "ACCEL", DXGI_FORMAT_R32G32B32_FLOAT });
	pipelineProp.inputLayoutProps.push_back({ "SCALES", DXGI_FORMAT_R32G32_FLOAT });
	pipelineProp.inputLayoutProps.push_back({ "NOWTIME", DXGI_FORMAT_R32G32_FLOAT });
	pipelineProp.inputLayoutProps.push_back({ "TYPE", DXGI_FORMAT_R8_UINT });
	pipelineProp.blendProp = { D3D12_BLEND_OP_ADD, D3D12_BLEND_ONE, D3D12_BLEND_ONE };
	pipelineProp.rootParamProp = { 1,1 };
	pipelineProp.depthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	pipelineProp.primitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	pipelineProp.cullMode = D3D12_CULL_MODE_BACK;
	pipelines[PipelineType::Particle].CreatePipeline(pipelineProp);
}

void PipelineManager::CreatePipeline(const PipelineProp& pipelineProp)
{
	ComPtr<ID3DBlob> vsBlob, gsBlob, psBlob;
	LoadShader(&vsBlob, pipelineProp.shaderNames[0], "vs_5_0");
	LoadShader(&psBlob, pipelineProp.shaderNames[1], "ps_5_0");
	if (!pipelineProp.shaderNames[2].empty()) { LoadShader(&gsBlob, pipelineProp.shaderNames[2], "gs_5_0"); }

	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;
	for (auto& inputLayoutProp : pipelineProp.inputLayoutProps)
	{
		inputLayout.push_back(SetInputLayout(inputLayoutProp.semanticName, inputLayoutProp.format));
	}

	std::vector<CD3DX12_ROOT_PARAMETER> rootParams;
	// デスクリプタレンジ
	for (UINT i = 0; i < pipelineProp.rootParamProp.descriptorNum; i++)
	{
		CD3DX12_DESCRIPTOR_RANGE* descriptorRange = new CD3DX12_DESCRIPTOR_RANGE();
		descriptorRange->Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, i);
		CD3DX12_ROOT_PARAMETER rootParam{};
		rootParam.InitAsDescriptorTable(1, descriptorRange);
		rootParams.push_back(rootParam);
	}

	for (UINT i = 0; i < pipelineProp.rootParamProp.constBuffNum; i++)
	{
		CD3DX12_ROOT_PARAMETER rootParam{};
		rootParam.InitAsConstantBufferView(i);
		rootParams.push_back(rootParam);
	}

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;
	blenddesc.BlendOp = pipelineProp.blendProp.blendOp;
	blenddesc.SrcBlend = pipelineProp.blendProp.srcBlend;
	blenddesc.DestBlend = pipelineProp.blendProp.destBlend;

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc(0, D3D12_FILTER_MIN_MAG_MIP_POINT,
		pipelineProp.textureAddressMode, pipelineProp.textureAddressMode);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Init_1_0((UINT)rootParams.size(), rootParams.data(), 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob, errorBlob;
	// バージョン自動判定のシリアライズ
	Result result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ルートシグネチャの生成
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));

	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipeline{};
	// グラフィックスパイプラインの流れを設定
	pipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	pipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());
	if (gsBlob) { pipeline.GS = CD3DX12_SHADER_BYTECODE(gsBlob.Get()); }
	// サンプルマスク
	pipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	pipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	pipeline.RasterizerState.CullMode = pipelineProp.cullMode;
	// 図形の形状設定
	pipeline.PrimitiveTopologyType = pipelineProp.primitiveTopologyType;
	// ブレンドステートの設定
	pipeline.BlendState.RenderTarget[0] = blenddesc;

	// 頂点レイアウトの設定
	pipeline.InputLayout.pInputElementDescs = inputLayout.data();
	pipeline.InputLayout.NumElements = (UINT)inputLayout.size();

	pipeline.NumRenderTargets = 1;	// 描画対象は1つ
	pipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	pipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// 3D用パイプライン
	if (pipelineProp.isDepthTest)
	{
		// デプスステンシルステート
		pipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		// 深度バッファのフォーマット
		pipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		pipeline.DepthStencilState.DepthWriteMask = pipelineProp.depthWriteMask;
	}

	pipeline.pRootSignature = rootSignature.Get();

	// グラフィックスパイプラインの生成
	result = device->CreateGraphicsPipelineState(&pipeline, IID_PPV_ARGS(&pipelineState));
}

void PipelineManager::SetPipeline(PipelineType type)
{
	// パイプラインステートとルートシグネチャの設定コマンド
	cmdList->SetPipelineState(pipelines[type].pipelineState.Get());
	cmdList->SetGraphicsRootSignature(pipelines[type].rootSignature.Get());
}