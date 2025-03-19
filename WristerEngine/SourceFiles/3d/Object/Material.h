#pragma once
#include "Sprite.h"
#include <sstream>

namespace WristerEngine::_3D
{
	class Mesh;
	struct Object3d;

	struct TextureTransform
	{
		Vector2 uvOffset;  // uv���炵
		Vector2 tiling = { 1,1 }; // �^�C�����O
	};

	// �e�N�X�`���̖���
	enum class TexType
	{
		Main,		// ���C���e�N�X�`��
		Sub,		// �T�u�e�N�X�`��
		Blend, 		// �u�����h�}�b�v
		Specular,	// �X�y�L�����}�b�v
		Dissolve,	// �f�B�]���u�}�b�v
		Num 		// �e�N�X�`����
	};

	struct MaterialData : public TextureTransform
	{
		ColorRGB color;
		_2D::TextureData* data = nullptr;
	};

	// �}�e���A��
	struct Material
	{
		std::string name;
		ColorRGB ambient;
		ColorRGB diffuse;
		ColorRGB specular;
		float alpha = 1.0f;
		std::array<MaterialData, (size_t)TexType::Num> textures; // �e�N�X�`���̔z��

		void ChangeTexture(size_t texIndex, const std::string& texName);

	private:
		friend Object3d;

		// �}�e���A��
		struct ConstBufferData
		{
			ColorRGBA ambient; // �p�f�B���O�܂�
			ColorRGBA diffuse;
			ColorRGBA specular;
			std::array<TextureTransform, 4> texTrans;
			std::array<ColorRGBA, 2> color; // �F
			std::array<float, 3> maskPow{}; // �}�X�N�̋���
		};

		Microsoft::WRL::ComPtr<ID3D12Resource> constBuffer;	// �萔�o�b�t�@
		ConstBufferData* constMap = nullptr;

		// �}�e���A���ǂݍ���
		void Load(Mesh* mesh);
		// �X�V
		void Update();
		// �`��
		void Draw();
		// �e�N�X�`���ǂݍ���
		void LoadTexture(std::istringstream& line_stream, Mesh* mesh, TexType spriteIndex);
	};
}