#include "Sprite.h"
#include "D3D12Common.h"
#include "SpriteManager.h"

using namespace WE;
using namespace _2D;

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

void Sprite::SetAnimation(size_t spriteNum, int animationIntervel)
{
	animation = std::make_unique<Animation>();
	animation->Initialize(this, spriteNum, animationIntervel);
}

void Sprite::Update()
{
	if (isInvisible) { return; }

	if (animation) { animation->Update(); }

	float left = (0.0f - anchorPoint.x);
	float right = (1.0f - anchorPoint.x);
	float top = (0.0f - anchorPoint.y);
	float bottom = (1.0f - anchorPoint.y);

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

	Transform::Update();

	// GPU転送
	constMap->mat = matWorld * SpriteManager::matProj;
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