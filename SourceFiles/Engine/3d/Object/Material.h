#pragma once
#include "Sprite.h"
#include <sstream>

namespace WristerEngine::_3D
{
	class Mesh;

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
		_2D::TextureData* tex = nullptr;
	};

	// �}�e���A��
	struct Material
	{
		std::string name;
		ColorRGB ambient = { 0.3f,0.3f,0.3f };
		ColorRGB diffuse;
		ColorRGB specular;
		std::array<MaterialData, (size_t)TexType::Num> textures; // �e�N�X�`���̔z��

		// �}�e���A���ǂݍ���
		void Load(Mesh* mesh);
		// �X�V
		void Update();
		// �`��
		void Draw();

	private:
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

		// �e�N�X�`���ǂݍ���
		void LoadTexture(std::istringstream& line_stream, Mesh* mesh, TexType spriteIndex);
		// �萔�o�b�t�@�ɓ]��
		void TransferCBV();
	};
}