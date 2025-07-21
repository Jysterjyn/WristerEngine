#include "Material.h"
#include <fstream>
#include "D3D12Common.h"
#include "Mesh.h"
#include "ModelManager.h"
#include "PipelineManager.h"
#include <StringUtility.h>
using namespace std;
using namespace WE;
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

void Material::LoadTexture(istringstream& line_stream, TexType spriteIndex)
{
	string textureFilename;
	line_stream >> textureFilename;
	textures[(size_t)spriteIndex].data = TextureData::Load(directoryPath + textureFilename);
}

void Material::ChangeTexture(size_t texIndex, const std::string& texName)
{
	// フルパス指定の場合
	if (texName.find("./") == std::string::npos) { textures[texIndex].data = TextureData::Load(texName); return; }

	// マテリアルが存在するディレクトリからの相対パス指定の場合
	std::string fileName = ExtractFileName(texName);
	textures[texIndex].data = TextureData::Load(directoryPath + fileName);
}

void Material::Load(Mesh* mesh)
{
	ifstream file;
	directoryPath = mesh->directoryPath;
	file.open(CreateResourcePath(directoryPath + mesh->materialFileName));
	assert(!file.fail());

	string line;
	while (getline(file, line))
	{
		istringstream line_stream(line);
		string key;
		getline(line_stream, key, ' ');

		if (key[0] == '\t') { key.erase(key.begin()); }
		if (key == "newmtl") { line_stream >> name; }
		if (key == "Ka") { LoadColorRGBStream(line_stream, ambient); }
		if (key == "Kd") { LoadColorRGBStream(line_stream, diffuse); }
		if (key == "Ks") { LoadColorRGBStream(line_stream, specular); }
		if (key == "map_Kd") { LoadTexture(line_stream, TexType::Main); }		// メインテクスチャ
		if (key == "map_Kds") { LoadTexture(line_stream, TexType::Sub); }		// サブテクスチャ
		if (key == "map_Kbm") { LoadTexture(line_stream, TexType::Blend); }		// ブレンドマスク
		if (key == "map_Ksm") { LoadTexture(line_stream, TexType::Specular); }	// スペキュラマスク
		if (key == "map_Kdm") { LoadTexture(line_stream, TexType::Dissolve); }	// ディゾルブマスク
	}
	file.close();

	// デフォルトテクスチャのセット
	for (auto& tex : textures) { if (!tex.data) { tex.data = TextureData::Load("white1x1.png"); } }

	// ブレンドテクスチャがデフォルトの場合、マスク値は使わない
	if (textures[(size_t)TexType::Blend].data->fileName.find("white1x1.png") != string::npos)
	{
		textures[(size_t)TexType::Blend].color.r = 0;
	}

	// ディゾルブ値の初期値は0
	textures[(size_t)TexType::Dissolve].color.r = 0;

	// 定数バッファ生成
	CreateBuffer(&constBuffer, &constMap, (sizeof(ConstBufferData) + 0xff) & ~0xff);
}

void Material::Update()
{
	for (size_t i = 0; i < constMap->texTrans.size(); i++) { constMap->texTrans[i] = textures[i]; }
	for (size_t i = 0; i < constMap->color.size(); i++) { constMap->color[i] = textures[i].color; }
	for (size_t i = 0; i < constMap->maskPow.size(); i++)
	{
		constMap->maskPow[i] = textures[(size_t)TexType::Blend + i].color.r;
	}

	constMap->ambient = { ambient,alpha };
	constMap->diffuse = diffuse;
	constMap->specular = specular;
}

void Material::Draw()
{
	if (constMap->maskPow[2] <= 0) { PipelineManager::SetPipeline(PipelineType::Object); }
	else { PipelineManager::SetPipeline(PipelineType::Dissolve); }
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
	cmdList->SetGraphicsRootConstantBufferView((UINT)RootParamNum::Material, constBuffer->GetGPUVirtualAddress());
	// シェーダリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(0, DirectXCommon::GetInstance()->GetSRV()->GetGPUDescriptorHandleForHeapStart());
	for (UINT i = 0; i < (UINT)TexType::Num; i++)
	{
		cmdList->SetGraphicsRootDescriptorTable(i, textures[i].data->srvHandle.gpu);
	}
}