#pragma once
#include "Sprite.h"
#include <sstream>

namespace WristerEngine::_3D
{
	class Mesh;
	struct Object3d;

	struct TextureTransform
	{
		Vector2 uvOffset;  // uvずらし
		Vector2 tiling = { 1,1 }; // タイリング
	};

	// テクスチャの役割
	enum class TexType
	{
		Main,		// メインテクスチャ
		Sub,		// サブテクスチャ
		Blend, 		// ブレンドマップ
		Specular,	// スペキュラマップ
		Dissolve,	// ディゾルブマップ
		Num 		// テクスチャ数
	};

	struct MaterialData : public TextureTransform
	{
		ColorRGB color;
		_2D::TextureData* data = nullptr;
	};

	// マテリアル
	struct Material
	{
		std::string name;
		ColorRGB ambient;
		ColorRGB diffuse;
		ColorRGB specular;
		float alpha = 1.0f;
		std::array<MaterialData, (size_t)TexType::Num> textures; // テクスチャの配列

		void ChangeTexture(size_t texIndex, const std::string& texName);

	private:
		friend Object3d;

		// マテリアル
		struct ConstBufferData
		{
			ColorRGBA ambient; // パディング含む
			ColorRGBA diffuse;
			ColorRGBA specular;
			std::array<TextureTransform, 4> texTrans;
			std::array<ColorRGBA, 2> color; // 色
			std::array<float, 3> maskPow{}; // マスクの強さ
		};

		Microsoft::WRL::ComPtr<ID3D12Resource> constBuffer;	// 定数バッファ
		ConstBufferData* constMap = nullptr;

		// マテリアル読み込み
		void Load(Mesh* mesh);
		// 更新
		void Update();
		// 描画
		void Draw();
		// テクスチャ読み込み
		void LoadTexture(std::istringstream& line_stream, Mesh* mesh, TexType spriteIndex);
	};
}