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

		uList<TextureData> textures;
		uList<Sprite> sprites;
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
		TextureData* LoadTexture(const std::string& fileName);

		// スプライト生成
		Sprite* Create(std::initializer_list<const std::string> fileNames,
			CR<Vector2> pos = {}, CR<Vector2> anchorPoint = {},
			CR<Vector2> textureSize = {}, CR<Vector2> textureLeftTop = {});

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