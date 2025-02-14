#pragma once
#include "Sprite.h"

namespace WristerEngine::_2D
{
	// マップチップ描画汎用クラス
	class BaseMapChip
	{
	protected:
		Vector2 leftTopPos; // 起点となる位置
		Vector2 chipSize; // チップ1枚のサイズ
		Vector2 chipAnchor; // チップのアンカーポイント
		std::vector<std::unique_ptr<Sprite>> sprites; // 描画に使うスプライト

	public:
		virtual ~BaseMapChip() = default;

		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="leftTopPos">起点となる位置</param>
		/// <param name="chipSize">チップ1枚のサイズ</param>
		/// <param name="chipAnchor">チップのアンカーポイント</param>
		void Initialize(const Vector2& leftTopPos, const Vector2& chipSize, const Vector2& chipAnchor);

		virtual void VirtualInitialize(Sprite* sprite, float splitNum) = 0;

		/// <summary>
		/// スプライト情報追加
		/// </summary>
		/// <param name="fileName">テクスチャファイル名</param>
		/// <param name="chipPos">チップ単位での場所</param>
		/// <param name="splitNum">分割数</param>
		/// <param name="drawSize">描画サイズ(0,0だと規定サイズ)</param>
		void AddSprite(const std::string& fileName, const Vector2& chipPos, float splitNum = 2.0f, const Vector2& drawSize = {});

		// 更新
		virtual void Update() = 0;
		// 描画
		void Draw();
	};
}