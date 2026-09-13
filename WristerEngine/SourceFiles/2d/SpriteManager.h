#pragma once
#include "DirectXCommon.h"
#include <Sprite.h>

namespace WristerEngine::_2D
{
	class SpriteManager final : DXCommonGetter
	{
	private:
		struct ConstBufferData
		{
			Matrix4 matProj;
		};

		Matrix4 OrthoGraphic();

		const Matrix4 matProj = OrthoGraphic();

		std::list<uPtr<TextureData>> textures;
		std::list<uPtr<Sprite>> sprites;
		Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;
		ConstBufferData* constMap = nullptr;

		SpriteManager() = default;
		~SpriteManager() = default;
		SpriteManager(const SpriteManager&) = delete;
		SpriteManager& operator=(const SpriteManager&) = delete;

	public:
		static SpriteManager* GetInstance();

		void Initialize();

		// テクスチャ読み込み
		TextureData* LoadTexture(CR<std::string> fileName);

		// スプライト生成
		Sprite* Create(CR<SpriteProp> prop);
		// スプライト生成(シンプル版)
		Sprite* Create(CR<std::string> fileName);

		void Update();

		// 描画前処理
		void PreDraw();
	};

	// 継承用
	struct SpriteUtility
	{
		SpriteManager* spMan = SpriteManager::GetInstance();
	};
}