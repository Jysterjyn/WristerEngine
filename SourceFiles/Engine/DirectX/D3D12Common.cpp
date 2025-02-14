#include "D3D12Common.h"
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
using namespace std;
using namespace WristerEngine;

map<PipelineType, PipelineManager> PipelineManager::pipelines;

D3D12_INPUT_ELEMENT_DESC WristerEngine::SetInputLayout(LPCSTR semanticName, DXGI_FORMAT format)
{
	D3D12_INPUT_ELEMENT_DESC inputLayout =
	{
		semanticName, 0, format, 0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	};
	return inputLayout;
}

void WristerEngine::LoadShader(ID3DBlob** shaderBlob, wstring shaderName, LPCSTR target)
{
	ID3DBlob* errorBlob = nullptr;

	wstring fileName = L"Resources/shaders/" + shaderName + L".hlsl";
	//シェーダの読み込みとコンパイル
	HRESULT result = D3DCompileFromFile(
		fileName.c_str(), // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", target, // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		shaderBlob, &errorBlob);
	// エラーなら
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		string error;
		error.resize(errorBlob->GetBufferSize());
		copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}
}

std::wstring WristerEngine::ConvertMultiByteStringToWideString(const std::string& mString)
{
	// ワイド文字列に変換した際の文字列バッファサイズを計算
	int filePathBufferSize = MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, nullptr, 0);
	// ワイド文字列に変換
	vector<wchar_t> wfilePath(filePathBufferSize);
	MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, wfilePath.data(), filePathBufferSize);

	return wfilePath.data();
}

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

	pipelineProp.shaderNames = { L"ObjVS", L"ObjPS" };
	pipelineProp.inputLayoutProps.clear();
	pipelineProp.inputLayoutProps.push_back({ "POSITION", DXGI_FORMAT_R32G32B32_FLOAT });
	pipelineProp.inputLayoutProps.push_back({ "NORMAL", DXGI_FORMAT_R32G32B32_FLOAT });
	pipelineProp.inputLayoutProps.push_back({ "TEXCOORD", DXGI_FORMAT_R32G32_FLOAT });
	pipelineProp.textureAddressMode = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	pipelineProp.isDepthTest = true;
	pipelineProp.rootParamProp = { 5,4 };
	pipelines[PipelineType::Object].CreatePipeline(pipelineProp);
	
	pipelineProp.cullMode = D3D12_CULL_MODE_NONE;
	pipelines[PipelineType::Dissolve].CreatePipeline(pipelineProp);
	
	pipelineProp.shaderNames = { L"ParticleVS", L"ParticlePS", L"ParticleGS" };
	pipelineProp.inputLayoutProps.clear();
	pipelineProp.inputLayoutProps.push_back({ "POSITION", DXGI_FORMAT_R32G32B32_FLOAT });
	pipelineProp.inputLayoutProps.push_back({ "TEXCOORD", DXGI_FORMAT_R32_FLOAT });
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
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_MIN_MAG_MIP_POINT,
		pipelineProp.textureAddressMode, pipelineProp.textureAddressMode);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Init_1_0((UINT)rootParams.size(), rootParams.data(), 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob, errorBlob;
	// バージョン自動判定のシリアライズ
	Result result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ルートシグネチャの生成
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();
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
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
	// パイプラインステートとルートシグネチャの設定コマンド
	cmdList->SetPipelineState(pipelines[type].pipelineState.Get());
	cmdList->SetGraphicsRootSignature(pipelines[type].rootSignature.Get());
}