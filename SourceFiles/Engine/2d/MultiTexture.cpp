#include "MultiTexture.h"
#include "D3D12Common.h"

using namespace WristerEngine::_2D;

#pragma region 生成関数
void MultiTexture::CreateBuffers()
{
	vertices =
	{ {
		{{-1,-1},{0,1}},
		{{-1,+1},{0,0}},
		{{+1,-1},{1,1}},
		{{+1,+1},{1,0}}
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
	CreateBuffer(constBuff.GetAddressOf(), &constMap, (sizeof(ConstBufferData) + 0xff) & ~0xff);
}
#pragma endregion

void MultiTexture::Initialize()
{
	CreateBuffers();
}

void MultiTexture::Draw(std::array<PostEffect, 2>& postEffects)
{
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();

	// パイプラインステートとルートシグネチャの設定コマンド
	PipelineManager::SetPipeline(PipelineType::MultiTexture);
	// プリミティブ形状の設定コマンド
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); // 三角形リスト
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	// デスクリプタヒープの設定コマンド
	ID3D12DescriptorHeap* ppHeaps[] = { PostEffect::GetSRV() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	cmdList->SetGraphicsRootDescriptorTable(1, postEffects[0].GetGPUHandle());
	cmdList->SetGraphicsRootDescriptorTable(2, postEffects[1].GetGPUHandle());

	// 頂点バッファビューの設定コマンド
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	// 描画コマンド
	cmdList->DrawInstanced((UINT)vertices.size(), 1, 0, 0); // 全ての頂点を使って描画
}