#pragma once
#include <D3D12Common.h>
#include <Color.h>

namespace WristerEngine::_3D
{
	class PrimitiveDrawer
	{
	private:
		// 線分の最大数
		static const UINT MAX_LINE_COUNT = 256;
		
		struct ConstBufferData
		{
			Matrix4 matCamera;
		};

		struct VertexData
		{
			Vector3 pos;
			ColorRGBA color;
		};

		std::vector<VertexData> vertices;	// 頂点データ配列
		Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;	// 頂点バッファ
		VertexData* vertMap = nullptr;	// 頂点バッファのマップ
		D3D12_VERTEX_BUFFER_VIEW vbView{};	// 頂点バッファビュー
		Microsoft::WRL::ComPtr<ID3D12Resource> constBuffer;	// 定数バッファ
		ConstBufferData* constMap = nullptr;

		PrimitiveDrawer() = default;
		~PrimitiveDrawer() = default;
		PrimitiveDrawer(const PrimitiveDrawer&) = delete;
		PrimitiveDrawer& operator=(const PrimitiveDrawer&) = delete;

	public:
		// シングルトンインスタンスの取得
		static PrimitiveDrawer* GetInstance();

		/// <summary>
		/// 3D線分の描画
		/// </summary>
		/// <param name="p1">始点座標</param>
		/// <param name="p2">終点座標</param>
		/// <param name="color">色(RGBA)</param>
		void DrawLine3d(const Vector3& p1, const Vector3& p2, const ColorRGBA& color);

		// 頂点データの転送
		void TransferVertices();

		// 初期化
		void Initialize();
		void Update();
		void Draw();
	};
}