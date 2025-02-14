#include "Material.h"
#include <fstream>
#include "D3D12Common.h"
#include "Mesh.h"
#include "ModelManager.h"
using namespace std;
using namespace WristerEngine;
using namespace _2D;
using namespace _3D;

void LoadColorRGBStream(istringstream& stream, ColorRGB& color)
{
	Vector3 colorTemp;
	stream >> colorTemp.x;
	stream >> colorTemp.y;
	stream >> colorTemp.z;
	color.r = colorTemp.x;
	color.g = colorTemp.y;
	color.b = colorTemp.z;
}

void Material::LoadSprite(istringstream& line_stream, Mesh* mesh, TexType spriteIndex)
{
	string textureFilename;
	line_stream >> textureFilename;
	string path = mesh->directoryPath;
	// スプライトのデフォルトディレクトリパスの文字列を削除
	string defaultDirectoryPath = Sprite::DEFAULT_TEXTURE_DIRECTORY_PATH;
	path.erase(path.begin(), path.begin() + defaultDirectoryPath.size());
	sprites[(size_t)spriteIndex] = Sprite::Create(path + textureFilename);
}

void Material::TransferCBV()
{
	for (size_t i = 0; i < constMap->texTrans.size(); i++)
	{
		constMap->texTrans[i].tiling =
		{
			sprites[i]->textureSize.x / sprites[i]->size.x,
			sprites[i]->textureSize.y / sprites[i]->size.y
		};

		if (sprites[i]->isFlipX) { constMap->texTrans[i].tiling.x *= -1.0f; }
		if (sprites[i]->isFlipY) { constMap->texTrans[i].tiling.y *= -1.0f; }

		constMap->texTrans[i].uvOffset =
		{
			sprites[i]->textureLeftTop.x / sprites[i]->size.x,
			sprites[i]->textureLeftTop.y / sprites[i]->size.y
		};
	}

	for (size_t i = 0; i < constMap->color.size(); i++) { constMap->color[i] = sprites[i]->color; }
	for (size_t i = 0; i < constMap->maskPow.size() - 1; i++)
	{
		constMap->maskPow[i] = sprites[(size_t)TexType::Blend + i]->color.r;
	}

	constMap->ambient = ambient;
	constMap->diffuse = diffuse;
	constMap->specular = specular;
}

void Material::Load(Mesh* mesh)
{
	ifstream file;
	file.open(mesh->directoryPath + mesh->materialFileName);
	assert(!file.fail());

	string line;
	while (getline(file, line))
	{
		istringstream line_stream(line);
		string key;
		getline(line_stream, key, ' ');

		if (key[0] == '\t') { key.erase(key.begin()); }
		if (key == "newmtl") { line_stream >> materialName; }
		if (key == "Ka") { LoadColorRGBStream(line_stream, ambient); }
		if (key == "Kd") { LoadColorRGBStream(line_stream, diffuse); }
		if (key == "Ks") { LoadColorRGBStream(line_stream, specular); }
		if (key == "map_Kd") { LoadSprite(line_stream, mesh, TexType::Main); }		// メインテクスチャ
		if (key == "map_Kds") { LoadSprite(line_stream, mesh, TexType::Sub); }		// サブテクスチャ
		if (key == "map_Kbm") { LoadSprite(line_stream, mesh, TexType::Blend); }	// ブレンドマスク
		if (key == "map_Ksm") { LoadSprite(line_stream, mesh, TexType::Specular); }	// スペキュラマスク
		if (key == "map_Kdm") { LoadSprite(line_stream, mesh, TexType::Dissolve); }	// ディゾルブマスク
	}
	file.close();

	// デフォルトテクスチャのセット
	for (auto& sprite : sprites) { if (!sprite) { sprite = Sprite::Create("white1x1.png"); } }

	// ブレンドテクスチャがデフォルトの場合、マスク値は使わない
	if (sprites[(size_t)TexType::Blend]->tex->fileName.find("white1x1.png") != string::npos)
	{
		sprites[(size_t)TexType::Blend]->color.r = 0;
	}

	// 定数バッファ生成
	CreateBuffer(&constBuffer, &constMap, (sizeof(ConstBufferData) + 0xff) & ~0xff);

	TransferCBV();
}

void Material::Update()
{
	for (auto& sprite : sprites) { sprite->Update(); }
	TransferCBV();
}

void Material::Draw()
{
	if (constMap->maskPow[2] <= 0) { PipelineManager::SetPipeline(PipelineType::Object); }
	else { PipelineManager::SetPipeline(PipelineType::Dissolve); }
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
	cmdList->SetGraphicsRootConstantBufferView((UINT)RootParamNum::Material, constBuffer->GetGPUVirtualAddress());
	// シェーダリソースビューをセット
	for (UINT i = 0; i < (UINT)TexType::Num; i++)
	{
		cmdList->SetGraphicsRootDescriptorTable(i, sprites[i]->GetGPUHandle());
	}
}