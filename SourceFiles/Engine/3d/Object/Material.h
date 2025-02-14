#pragma once
#include "Sprite.h"
#include <sstream>

namespace WristerEngine::_3D
{
	class Mesh;

	struct TextureTransform
	{
		Vector2 uvOffset;  // uvずらし
		Vector2 tiling; // タイリング
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

	// マテリアル
	struct Material
	{
		std::string materialName;
		ColorRGB ambient = { 0.3f,0.3f,0.3f };
		ColorRGB diffuse;
		ColorRGB specular;

		// マテリアル読み込み
		void Load(Mesh* mesh);
		// 更新
		void Update();
		// 描画
		void Draw();
		// 定数バッファに転送
		void TransferCBV();
		// setter
		void SetDissolvePow(float dissolve) { constMap->maskPow[2] = dissolve; }
		void SetSprite(std::unique_ptr<_2D::Sprite> sprite, TexType type) { sprites[(size_t)type] = move(sprite); }
		// getter
		_2D::Sprite* GetSprite(TexType texType) { return sprites[(size_t)texType].get(); }

	private:
		// マテリアル
		struct ConstBufferData
		{
			ColorRGBA ambient; // パディング含む
			ColorRGBA diffuse;
			ColorRGBA specular;
			std::array<TextureTransform, 4> texTrans;
			std::array<ColorRGBA, 2> color; // 色
			std::array<float, 3> maskPow; // マスクの強さ
		};

		Microsoft::WRL::ComPtr<ID3D12Resource> constBuffer;	// 定数バッファ
		ConstBufferData* constMap = nullptr;

		std::array<std::unique_ptr<_2D::Sprite>, (size_t)TexType::Num> sprites; // テクスチャの配列
		// テクスチャ読み込み
		void LoadSprite(std::istringstream& line_stream, Mesh* mesh, TexType spriteIndex);
	};
}