#pragma once

namespace WristerEngine
{
	// 1色分の構造体
	struct Color
	{
	public:
		// カラーの最大値
		static const float MAX;

	private:
		float val_ = Color::MAX;

	public:
		Color() = default;
		Color(float color) { val_ = color; }
		// 演算子オーバーロード
		float operator=(float val);
		Color operator*=(float rate) { val_ *= rate; return val_; }
		void operator+=(float val) { operator=(val_ + val); }
		void operator-=(float val) { operator=(val_ - val); }
		operator float() { return val_; }
	};

	// カラー値RGB
	struct ColorRGB
	{
		Color r, g, b;
		// コンストラクタ
		ColorRGB() = default;
		ColorRGB(Color red, Color green, Color blue) { r = red, g = green, b = blue; }

		// 演算子オーバーロード
		ColorRGB& operator*=(float rate);

		// 色取得
		static ColorRGB Red() { return { 1,0,0 }; }
		static ColorRGB Green() { return { 0,1,0 }; }
		static ColorRGB Blue() { return { 0,0,1 }; }
		static ColorRGB White() { return { 1,1,1 }; }
		static ColorRGB Black() { return { 0,0,0 }; }
		static ColorRGB Yellow() { return { 1,1,0 }; }
		static ColorRGB Magenta() { return { 1,0,1 }; }
		static ColorRGB Cyan() { return { 0,1,1 }; }
	};

	// カラー値RGBA
	struct ColorRGBA
	{
		Color r, g, b, a;
		// コンストラクタ
		ColorRGBA() {}
		ColorRGBA(Color red, Color green, Color blue, Color alpha = Color::MAX) { r = red, g = green, b = blue, a = alpha; }
		ColorRGBA(ColorRGB color, float alpha = Color::MAX) { r = color.r, g = color.g, b = color.b, a = alpha; }

		// 演算子オーバーロード
		ColorRGBA& operator*=(float rate);

		// 色取得
		static ColorRGBA Red() { return { 1,0,0,1 }; }
		static ColorRGBA Green() { return { 0,1,0,1 }; }
		static ColorRGBA Blue() { return { 0,0,1,1 }; }
		static ColorRGBA White() { return { 1,1,1,1 }; }
		static ColorRGBA Black() { return { 0,0,0,1 }; }
		static ColorRGBA Yellow() { return { 1,1,0,1 }; }
		static ColorRGBA Magenta() { return { 1,0,1,1 }; }
		static ColorRGBA Cyan() { return { 0,1,1,1 }; }
	};

	// 2項演算子オーバーロード
	const Color operator*(Color color, float rate);
	const ColorRGB operator*(ColorRGB color, float rate);
	const ColorRGBA operator*(ColorRGBA color, float rate);
}