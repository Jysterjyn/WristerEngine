#pragma once
#include <array>
#include <memory>
#include "Color.h"
#include "DirectXCommon.h"
#include "Transform.h"

namespace WristerEngine::_2D
{
	// テクスチャ1枚分のデータ
	struct TextureData
	{
		std::string fileName;
		Microsoft::WRL::ComPtr<ID3D12Resource> buffer;
		SRVHandle srvHandle;
	};

	struct SpriteProp
	{
		// Sprite汎用変数
		std::vector<std::string> fileNames;
		Vector2 pos;
		Vector2 anchorPoint;
		Vector2 textureSize;
		Vector2 textureLeftTop;
		// SpriteAnimation汎用変数
		bool isAnimation = false;
		float width = 0;
		int interval = 1;
		size_t spriteNum = 0;

		void AddFileNames(const std::vector<std::string>& fileNames);
	};

	class SpriteManager;

	// スプライト
	class Sprite : private DXCommonGetter, public Transform
	{
	public:
		ColorRGBA color;
		Vector2 anchorPoint;
		bool isFlipX = false;
		bool isFlipY = false;
		bool isInvisible = false;
		Vector2 textureLeftTop; // 切り取り領域の左上座標
		Vector2 textureSize; // 切り取り領域のサイズ
		Vector2 posOffset; // 表示位置の調整

	private:
		friend SpriteManager;

		struct ConstBufferData
		{
			Matrix4 mat;
			ColorRGBA color;
		};

		enum class VertexNumber
		{
			LB, // 左下
			LT, // 左上
			RB, // 右下
			RT  // 右上
		};

		template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

		std::array<Vertex, 4> vertices;
		D3D12_VERTEX_BUFFER_VIEW vbView{};
		Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;
		ConstBufferData* constMap = nullptr;
		Vertex* vertMap = nullptr;
		UINT16 texIndex = 0;
		std::vector<TextureData*> textures{};

		// テクスチャサイズをイメージに合わせる
		void AdjustTextureSize();

	protected:
		// 初期化
		virtual void Initialize(CR<SpriteProp> prop);
		// 更新
		virtual void Update();

	public:
		// 描画
		void Draw();
		// 位置を画面中央にする
		void SetCenterPos() { position = Half(WIN_SIZE); }
		// anchorPoint = { 0.5f,0.5f } にする
		void SetCenterAnchor() { anchorPoint = { 0.5f,0.5f }; }

		// 現在のインデックスが示すテクスチャのGPUハンドルを取得
		D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle() const { return textures[texIndex]->srvHandle.gpu; }

		/// <summary>
		/// 切り取り領域を指定(描画サイズも変える)
		/// </summary>
		/// <param name="textureSize">切り取り領域のサイズ</param>
		/// <param name="textureLeftTop">切り取り領域の左上座標</param>
		void SetRect(CR<Vector2> textureSize, CR<Vector2> textureLeftTop = {});
		
		/// <summary>
		/// テクスチャ分割
		/// </summary>
		/// <param name="spritNum">分割数</param>
		void Split(CR<Vector2> spritNum);

		void SetTextureIndex(UINT16 texIndex);
	};

	class SpriteAnimation : public Sprite
	{
	private:
		float width = 0;
		FrameTimer interval;
		size_t animeNum = 0;
		size_t animeNumMax = 0;

	public:
		// 初期化
		void Initialize(CR<SpriteProp> prop) override;
		// 更新
		void Update() override;
	};
}