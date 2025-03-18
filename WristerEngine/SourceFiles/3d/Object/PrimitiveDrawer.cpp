#include "PrimitiveDrawer.h"
#include "ModelManager.h"
using namespace WristerEngine::_3D;

PrimitiveDrawer* PrimitiveDrawer::GetInstance()
{
	static PrimitiveDrawer instance;
	return &instance;
}

void PrimitiveDrawer::DrawLine3d(const Vector3& p1, const Vector3& p2, const ColorRGBA& color)
{
	vertices.push_back({ p1,color });
	vertices.push_back({ p2,color });
}

void PrimitiveDrawer::TransferVertices()
{
	copy(vertices.begin(), vertices.end(), vertMap);
}

void PrimitiveDrawer::Initialize()
{
	UINT sizeVB = static_cast<UINT>(sizeof(VertexData) * MAX_LINE_COUNT);
	// 頂点バッファ生成
	CreateBuffer(&vertBuff, &vertMap, sizeVB);
	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(VertexData);

	CreateBuffer(&constBuffer, &constMap, (sizeof(ConstBufferData) + 0xff) & ~0xff);
}

void PrimitiveDrawer::Update()
{
	constMap->matCamera = ModelManager::GetInstance()->GetCamera()->GetViewProjectionMatrix();
}

void PrimitiveDrawer::Draw()
{
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
	PipelineManager::SetPipeline(PipelineType::Primitive);
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffer->GetGPUVirtualAddress());
	cmdList->DrawInstanced((UINT)vertices.size(), 1, 0, 0);
}