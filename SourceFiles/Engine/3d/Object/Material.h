#pragma once
#include "Sprite.h"
#include <sstream>

namespace WristerEngine::_3D
{
	class Mesh;

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
		_2D::TextureData* tex = nullptr;
	};

	// マテリアル
	struct Material
	{
		std::string name;
		ColorRGB ambient = { 0.3f,0.3f,0.3f };
		ColorRGB diffuse;
		ColorRGB specular;
		std::array<MaterialData, (size_t)TexType::Num> textures; // テクスチャの配列

		// マテリアル読み込み
		void Load(Mesh* mesh);
		// 更新
		void Update();
		// 描画
		void Draw();

	private:
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

		// テクスチャ読み込み
		void LoadTexture(std::istringstream& line_stream, Mesh* mesh, TexType spriteIndex);
		// 定数バッファに転送
		void TransferCBV();
	};
}