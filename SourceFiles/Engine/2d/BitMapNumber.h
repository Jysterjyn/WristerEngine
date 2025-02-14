#pragma once
#include <vector>
#include "Sprite.h"

namespace WristerEngine::_2D
{
	// ビットマップの読み取り変数の受け渡し構造体
	struct BitMapProp
	{
		// ビットマップがある画像のファイル名
		std::string fileName;
		// 1文字分のサイズ
		Vector2 rectSize;
		// 描画サイズ
		Vector2 size;
		// 描画位置(ピクセル座標)
		Vector2 pos;
		// 表示桁数
		int digit = 0;
		// 色合い
		ColorRGBA color;
		// 切り取り領域の左上座標のオフセット
		Vector2 texLTOffset;
		// アンカーポイント
		Vector2 anchorPoint;

		/// <summary>
		/// BitMapNumber初期化に必要な値のセット
		/// </summary>
		/// <param name="fileName">ビットマップがある画像のファイル名</param>
		/// <param name="rectSize">1文字分のサイズ</param>
		/// <param name="size">描画サイズ</param>
		/// <param name="pos">描画位置(ピクセル座標)</param>
		/// <param name="digit">表示桁数</param>
		void SetVal(const std::string& fileName, Vector2 rectSize, Vector2 size, Vector2 pos, int digit);
	};

	/// <summary>
	/// ビットマップの数字の描画
	/// 整数にのみ対応
	/// </summary>
	class BitMapNumber
	{
	private:
		std::vector<std::unique_ptr<WristerEngine::_2D::Sprite>> sprites;
		BitMapProp bitMapProp;
		Vector2 allSpriteSize; // スプライト全体の大きさ

		/// <summary>
		/// スプライトの更新(Initialize,Update関数の共通部分)
		/// </summary>
		/// <param name="index">spritesのインデックス</param>
		void UpdateSprite(size_t index);

	public:
		// 初期化
		void Initialize();

		/// <summary>
		/// 更新
		/// </summary>
		/// <param name="number">描画する数字</param>
		void Update(int number);
		// 描画
		void Draw();
		/// <summary>
		/// bitMapPropを取得
		/// </summary>
		/// <returns>bitMapPropのポインタ</returns>
		BitMapProp* GetBitMapProp() { return &bitMapProp; }
		// スプライト全体の大きさを取得
		Vector2 GetAllSpriteSize() const { return allSpriteSize; }
	};
}