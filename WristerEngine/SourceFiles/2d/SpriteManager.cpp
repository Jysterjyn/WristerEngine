#include "SpriteManager.h"
#include <DirectXTex.h>
#include "D3D12Common.h"
#include <StringUtility.h>
#include "PipelineManager.h"

using namespace WE;
using namespace _2D;
using namespace DirectX;

// 平行投影行列
Matrix4 SpriteManager::OrthoGraphic()
{
	Matrix4 matProj_;
	// 平行投影行列の生成
	matProj_.m[0][0] = 2.0f / WIN_SIZE.x;
	matProj_.m[1][1] = -2.0f / WIN_SIZE.y;
	matProj_.m[3][0] = -1.0f;
	matProj_.m[3][1] = 1.0f;
	return matProj_;
}

SpriteManager* SpriteManager::GetInstance()
{
	static SpriteManager instance;
	return &instance;
}

void SpriteManager::Initialize()
{
	// 定数バッファ
	CreateBuffer(constBuff.GetAddressOf(), &constMap, (sizeof(ConstBufferData) + 0xff) & ~0xff);
	constMap->matProj = matProj;
}

TextureData* SpriteManager::LoadTexture(const std::string& fileName)
{
	// テクスチャの重複読み込みを検出
	for (auto& tex : textures)
	{
		if (tex->fileName.find(fileName) == std::string::npos) { continue; }
		return tex.get();
	}

	TexMetadata metadata{};
	ScratchImage scratchImg{}, mipChain{};

	std::string fullPath = CreateResourcePath(fileName);

	// ワイド文字列に変換
	std::wstring wfilePath = ConvertMultiByteStringToWideString(fullPath);

	Result result = S_OK;
	bool isDDSFile = fileName.find(".dds") != std::string::npos;

	if (isDDSFile)
	{
		result = LoadFromDDSFile(wfilePath.c_str(), DDS_FLAGS_NONE, &metadata, scratchImg);
	}
	else
	{
		result = LoadFromWICFile(wfilePath.c_str(), WIC_FLAGS_FORCE_SRGB, &metadata, scratchImg);

		HRESULT result1 = GenerateMipMaps(scratchImg.GetImages(), scratchImg.GetImageCount(),
			scratchImg.GetMetadata(), TEX_FILTER_DEFAULT, 0, mipChain);
		if (SUCCEEDED(result1))
		{
			scratchImg = std::move(mipChain);
			metadata = scratchImg.GetMetadata();
		}
	}

	D3D12_RESOURCE_DESC textureResourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format, metadata.width, (UINT)metadata.height,
		(UINT16)metadata.arraySize, (UINT16)metadata.mipLevels);

	std::unique_ptr<TextureData> texture = std::make_unique<TextureData>();
	CD3DX12_HEAP_PROPERTIES heapProp(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);

	result = device->CreateCommittedResource(
		&heapProp, D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(&texture->buffer));

	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		const Image* img = scratchImg.GetImage(i, 0, 0);
		result = texture->buffer->WriteToSubresource((UINT)i, nullptr, img->pixels,
			(UINT)img->rowPitch, (UINT)img->slicePitch);
	}

	texture->fileName = fileName;
	texture->srvHandle = dxCommon->CreateSRV(texture->buffer.Get(), &textureResourceDesc);
	textures.push_back(move(texture));
	return textures.back().get();
}

Sprite* SpriteManager::Create(std::initializer_list<const std::string> fileNames,
	CR<Vector2> pos, CR<Vector2> anchorPoint,
	CR<Vector2> textureSize, CR<Vector2> textureLeftTop)
{
	std::unique_ptr<Sprite> sprite = std::make_unique<Sprite>();
	for (const std::string& fileName : fileNames)
	{
		TextureData* tex = LoadTexture(fileName);
		sprite->textures.push_back(tex);
	}
	sprite->Initialize();
	sprite->position = pos;
	sprite->anchorPoint = anchorPoint;
	if (textureSize.Length() != 0) { sprite->SetRect(textureSize, textureLeftTop); }
	sprites.push_back(move(sprite));
	return sprites.back().get();
}

void SpriteManager::PreDraw()
{
	// パイプラインステートとルートシグネチャの設定コマンド
	PipelineManager::SetPipeline(PipelineType::Sprite);
	// プリミティブ形状の設定コマンド
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); // 三角形リスト
	cmdList->SetGraphicsRootConstantBufferView(2, constBuff->GetGPUVirtualAddress());
}

void SpriteManager::Update()
{
	for (auto& s : sprites) { s->Update(); }
}
