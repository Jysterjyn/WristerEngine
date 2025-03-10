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

void Material::LoadTexture(istringstream& line_stream, Mesh* mesh, TexType spriteIndex)
{
	string textureFilename;
	line_stream >> textureFilename;
	string path = mesh->directoryPath;
	// �X�v���C�g�̃f�t�H���g�f�B���N�g���p�X�̕�������폜
	string defaultDirectoryPath = TextureData::DEFAULT_TEXTURE_DIRECTORY_PATH;
	path.erase(path.begin(), path.begin() + defaultDirectoryPath.size());
	textures[(size_t)spriteIndex].data = TextureData::Load(path + textureFilename);
}

void Material::TransferCBV()
{
	for (size_t i = 0; i < constMap->texTrans.size(); i++) { constMap->texTrans[i] = textures[i]; }
	for (size_t i = 0; i < constMap->color.size(); i++) { constMap->color[i] = textures[i].color; }
	for (size_t i = 0; i < constMap->maskPow.size(); i++)
	{
		constMap->maskPow[i] = textures[(size_t)TexType::Blend + i].color.r;
	}

	constMap->ambient = ambient;
	constMap->diffuse = diffuse;
	constMap->specular = specular;
}

void Material::ChangeTexture(size_t texIndex, const std::string& texName)
{
	textures[texIndex].data = TextureData::Load(texName);
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
		if (key == "newmtl") { line_stream >> name; }
		if (key == "Ka") { LoadColorRGBStream(line_stream, ambient); }
		if (key == "Kd") { LoadColorRGBStream(line_stream, diffuse); }
		if (key == "Ks") { LoadColorRGBStream(line_stream, specular); }
		if (key == "map_Kd") { LoadTexture(line_stream, mesh, TexType::Main); }		// ���C���e�N�X�`��
		if (key == "map_Kds") { LoadTexture(line_stream, mesh, TexType::Sub); }		// �T�u�e�N�X�`��
		if (key == "map_Kbm") { LoadTexture(line_stream, mesh, TexType::Blend); }	// �u�����h�}�X�N
		if (key == "map_Ksm") { LoadTexture(line_stream, mesh, TexType::Specular); }	// �X�y�L�����}�X�N
		if (key == "map_Kdm") { LoadTexture(line_stream, mesh, TexType::Dissolve); }	// �f�B�]���u�}�X�N
	}
	file.close();

	// �f�t�H���g�e�N�X�`���̃Z�b�g
	for (auto& tex : textures) { if (!tex.data) { tex.data = TextureData::Load("white1x1.png"); } }

	// �u�����h�e�N�X�`�����f�t�H���g�̏ꍇ�A�}�X�N�l�͎g��Ȃ�
	if (textures[(size_t)TexType::Blend].data->fileName.find("white1x1.png") != string::npos)
	{
		textures[(size_t)TexType::Blend].color.r = 0;
	}

	// �f�B�]���u�l�̏����l��0
	textures[(size_t)TexType::Dissolve].color.r = 0;

	// �萔�o�b�t�@����
	CreateBuffer(&constBuffer, &constMap, (sizeof(ConstBufferData) + 0xff) & ~0xff);

	TransferCBV();
}

void Material::Update()
{
	TransferCBV();
}

void Material::Draw()
{
	if (constMap->maskPow[2] <= 0) { PipelineManager::SetPipeline(PipelineType::Object); }
	else { PipelineManager::SetPipeline(PipelineType::Dissolve); }
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
	cmdList->SetGraphicsRootConstantBufferView((UINT)RootParamNum::Material, constBuffer->GetGPUVirtualAddress());
	// �V�F�[�_���\�[�X�r���[���Z�b�g
	for (UINT i = 0; i < (UINT)TexType::Num; i++)
	{
		cmdList->SetGraphicsRootDescriptorTable(i, textures[i].data->srvHandle.gpu);
	}
}