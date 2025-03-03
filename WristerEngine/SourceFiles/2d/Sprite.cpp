#include "Sprite.h"
#include "D3D12Common.h"
#include <DirectXTex.h>
#include <StringUtility.h>
using namespace std;
using namespace DirectX;
using namespace WristerEngine;
using namespace _2D;

// ���s���e�s��
static Matrix4 OrthoGraphic()
{
	Matrix4 matProj;
	// ���s���e�s��̐���
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
	// �g��䗦��ۂ����܂ܐ؂���̈��ύX����
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
	// �e�N�X�`�����𒴂��Ă����~
	assert(texIndex_ < textures.size());
	texIndex = texIndex_;
}

TextureData* TextureData::Load(const std::string& fileName)
{
	// �e�N�X�`���̏d���ǂݍ��݂����o
	for (auto& tex : textures)
	{
		if (tex->fileName.find(fileName) == string::npos) { continue; }
		return tex.get();
	}

	TexMetadata metadata{};
	ScratchImage scratchImg{}, mipChain{};

	string fullPath = DEFAULT_TEXTURE_DIRECTORY_PATH + fileName;

	// ���C�h������ɕϊ�
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
	// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	PipelineManager::SetPipeline(PipelineType::Sprite);
	// �v���~�e�B�u�`��̐ݒ�R�}���h
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); // �O�p�`���X�g
}

void Sprite::Initialize()
{
	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * vertices.size());
	ID3D12Resource* vertBuff = nullptr;
	CreateBuffer(&vertBuff, &vertMap, sizeVB);

	// �S���_�ɑ΂��č��W���R�s�[
	copy(vertices.begin(), vertices.end(), vertMap);

	// GPU���z�A�h���X
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	vbView.SizeInBytes = sizeVB;
	// ���_1���̃f�[�^�T�C�Y
	vbView.StrideInBytes = sizeof(Vertex);

	// �萔�o�b�t�@
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

	// GPU�]��
	constMap->mat = matWorld * matProj;
	constMap->color = color;
	std::copy(std::begin(vertices), std::end(vertices), vertMap);
}

void Sprite::Draw()
{
	if (isInvisible) { return; }

	cmdList->SetGraphicsRootDescriptorTable(0, GetGPUHandle());

	// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	cmdList->SetGraphicsRootConstantBufferView(1, constBuff->GetGPUVirtualAddress());
	// �`��R�}���h
	cmdList->DrawInstanced((UINT)vertices.size(), 1, 0, 0); // �S�Ă̒��_���g���ĕ`��
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