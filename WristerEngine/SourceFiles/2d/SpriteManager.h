#pragma once
#include "DirectXCommon.h"
#include <Sprite.h>

namespace WristerEngine::_2D
{
	class SpriteManager final : DXCommonGetter
	{
	private:
		uList<TextureData> textures;
		uList<Sprite> sprites;

		SpriteManager() = default;
		~SpriteManager() = default;
		SpriteManager(const SpriteManager&) = delete;
		SpriteManager& operator=(const SpriteManager&) = delete;

	public:
		static SpriteManager* GetInstance();

		static const Matrix4 matProj;

		// テクスチャ読み込み
		TextureData* LoadTexture(const std::string& fileName);

		// スプライト生成
		Sprite* Create(std::initializer_list<const std::string> fileNames,
			const Vector2& pos = {}, const Vector2& anchorPoint = {},
			const Vector2& textureSize = {}, const Vector2& textureLeftTop = {});
		
		void UpdateAll();

		// 描画前処理
		void PreDraw();
	};

	// 継承用
	struct SpriteUtility
	{
		SpriteManager* spMan = SpriteManager::GetInstance();
	};
}