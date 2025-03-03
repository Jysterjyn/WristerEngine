#include "Sprite.h"
#include "D3D12Common.h"
#include <DirectXTex.h>
#include <StringUtility.h>
using namespace std;
using namespace DirectX;
using namespace WristerEngine;
using namespace _2D;

// 平行投影行列
static Matrix4 OrthoGraphic()
{
	Matrix4 matProj;
	// 平行投影行列の生成
	matProj.m[0][0] = 2.0f / WIN_SIZE.x;
	matProj.m[1][1] = -2.0f / WIN_SIZE.y;
	matProj.m[3][0] = -1.0f;
	matProj.m[3][1] = 1.0f;
	return matProj;
}

string TextureData::DEFAULT_TEXTURE_DIRECTORY_PATH = "Resources/";
list<unique_ptr<TextureData>> TextureData::textures;
list<unique_ptr<Sprite>> Sprite::sprites;
const Matrix4 Sprite::matProj = OrthoGraphic();

void Sprite::SetRect(const Vector2& textureSize_, const Vector2& textureLeftTop_)
{
	// 拡大比率を保ったまま切り取り領域を変更する
	Vector2 sizeRate = { size.x / textureSize.x ,size.y / textureSize.y };
	size = textureSize = textureSize_;
	size.x *= sizeRate.x;
	size.y *= sizeRate.y;
	textureLeftTop = textureLeftTop_;
}

void Sprite::Split(const Vector2& spritNum)
{
	textureSize.x /= spritNum.x;
	textureSize.y /= spritNum.y;
	size.x /= spritNum.x;
	size.y /= spritNum.y;
}

void Sprite::SetTextureIndex(UINT16 texIndex_)
{
	// テクスチャ数を超えてたら停止
	assert(texIndex_ < textures.size());
	texIndex = texIndex_;
}

TextureData* TextureData::Load(const std::string& fileName)
{
	// テクスチャの重複読み込みを検出
	for (auto& tex : textures)
	{
		if (tex->fileName.find(fileName) == string::npos) { continue; }
		return tex.get();
	}

	TexMetadata metadata{};
	ScratchImage scratchImg{}, mipChain{};

	string fullPath = DEFAULT_TEXTURE_DIRECTORY_PATH + fileName;

	// ワイド文字列に変換
	std::wstring wfilePath = ConvertMultiByteStringToWideString(fullPath);

	Result result = S_OK;
	bool isDDSFile = fileName.find(".dds") != string::npos;

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
			scratchImg = move(mipChain);
			metadata = scratchImg.GetMetadata();
		}
	}

	D3D12_RESOURCE_DESC textureResourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format, metadata.width, (UINT)metadata.height,
		(UINT16)metadata.arraySize, (UINT16)metadata.mipLevels);

	unique_ptr<TextureData> texture = make_unique<TextureData>();
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

Sprite* Sprite::Create(std::initializer_list<const std::string> fileNames,
	const Vector2& pos, const Vector2& anchorPoint,
	const Vector2& textureSize, const Vector2& textureLeftTop)
{
	std::unique_ptr<Sprite> sprite = std::make_unique<Sprite>();
	for (const std::string& fileName : fileNames)
	{
		TextureData* tex = TextureData::Load(fileName);
		sprite->textures.push_back(tex);
	}
	sprite->Initialize();
	sprite->position = pos;
	sprite->anchorPoint = anchorPoint;
	if (textureSize.Length() != 0) { sprite->SetRect(textureSize, textureLeftTop); }
	sprites.push_back(move(sprite));
	return sprites.back().get();
}

void Sprite::PreDraw()
{
	// パイプラインステートとルートシグネチャの設定コマンド
	PipelineManager::SetPipeline(PipelineType::Sprite);
	// プリミティブ形状の設定コマンド
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); // 三角形リスト
}

void Sprite::Initialize()
{
	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * vertices.size());
	ID3D12Resource* vertBuff = nullptr;
	CreateBuffer(&vertBuff, &vertMap, sizeVB);

	// 全頂点に対して座標をコピー
	copy(vertices.begin(), vertices.end(), vertMap);

	// GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vbView.SizeInBytes = sizeVB;
	// 頂点1つ分のデータサイズ
	vbView.StrideInBytes = sizeof(Vertex);

	// 定数バッファ
	CreateBuffer(constBuff.GetAddressOf(), &constMap, (sizeof(ConstBufferData) + 0xff) & ~0xff);

	AdjustTextureSize();
	size = textureSize;
}

void Sprite::AdjustTextureSize()
{
	assert(textures[0]->buffer);

	D3D12_RESOURCE_DESC resDesc = textures[0]->buffer->GetDesc();

	textureSize.x = static_cast<float>(resDesc.Width);
	textureSize.y = static_cast<float>(resDesc.Height);
}

void WristerEngine::_2D::Sprite::UpdateAll()
{
	for (auto& s : sprites) { s->Update(); }
}

void Sprite::SetAnimation(size_t spriteNum, int animationIntervel)
{
	animation = std::make_unique<Animation>();
	animation->Initialize(this, spriteNum, animationIntervel);
}

void Sprite::Update()
{
	if (isInvisible) { return; }

	if (animation) { animation->Update(); }

	float left = (0.0f - anchorPoint.x) * size.x;
	float right = (1.0f - anchorPoint.x) * size.x;
	float top = (0.0f - anchorPoint.y) * size.y;
	float bottom = (1.0f - anchorPoint.y) * size.y;

	if (isFlipX) { left = -left; right = -right; }
	if (isFlipY) { top = -top; bottom = -bottom; }

	vertices[(size_t)VertexNumber::LB].pos = { left, bottom };
	vertices[(size_t)VertexNumber::LT].pos = { left, top };
	vertices[(size_t)VertexNumber::RB].pos = { right, bottom };
	vertices[(size_t)VertexNumber::RT].pos = { right, top };

	D3D12_RESOURCE_DESC resDesc = textures[texIndex]->buffer->GetDesc();

	float tex_left = textureLeftTop.x / resDesc.Width;
	float tex_right = (textureLeftTop.x + textureSize.x) / resDesc.Width;
	float tex_top = textureLeftTop.y / resDesc.Height;
	float tex_bottom = (textureLeftTop.y + textureSize.y) / resDesc.Height;

	vertices[(size_t)VertexNumber::LB].uv = { tex_left, tex_bottom };
	vertices[(size_t)VertexNumber::LT].uv = { tex_left, tex_top };
	vertices[(size_t)VertexNumber::RB].uv = { tex_right, tex_bottom };
	vertices[(size_t)VertexNumber::RT].uv = { tex_right, tex_top };

	Matrix4 matRot, matTrans;
	matRot = Matrix4::RotateZ(rotation);
	matTrans = Matrix4::Translate(position + posOffset);

	matWorld = matRot * matTrans;

	// GPU転送
	constMap->mat = matWorld * matProj;
	constMap->color = color;
	std::copy(std::begin(vertices), std::end(vertices), vertMap);
}

void Sprite::Draw()
{
	if (isInvisible) { return; }

	cmdList->SetGraphicsRootDescriptorTable(0, GetGPUHandle());

	// 頂点バッファビューの設定コマンド
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	cmdList->SetGraphicsRootConstantBufferView(1, constBuff->GetGPUVirtualAddress());
	// 描画コマンド
	cmdList->DrawInstanced((UINT)vertices.size(), 1, 0, 0); // 全ての頂点を使って描画
}

void Sprite::Animation::Initialize(Sprite* sprite_, size_t spriteNum, int animationIntervel)
{
	sprite = sprite_;
	width = sprite->textureSize.x / spriteNum;
	interval = animationIntervel;
	animeNumMax = spriteNum;
	sprite->SetRect({ width,sprite->textureSize.y });
}

void Sprite::Animation::Update()
{
	if (!interval.Update()) { return; }
	animeNum = NumberLoop(animeNum + 1, animeNumMax);
	sprite->textureLeftTop = { (float)animeNum * width ,0 };
}