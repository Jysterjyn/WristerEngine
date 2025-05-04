#include "Color.h"
using namespace WE;

const float Color::MAX = 1.0f;

float Color::operator=(float val)
{
	val_ = val;
	if (val_ > Color::MAX) { val_ -= (int)val_; }
	if (val_ < 0.0f) { val_ += Color::MAX + (int)val_; }
	return val_;
}

ColorRGB& ColorRGB::operator*=(float rate)
{
	r *= rate;
	g *= rate;
	b *= rate;
	return *this;
}

ColorRGBA& ColorRGBA::operator*=(float rate)
{
	r *= rate;
	g *= rate;
	b *= rate;
	a *= rate;
	return *this;
}

const Color operator*(Color color, float rate)
{
	return Color(color *= rate);
}

const ColorRGB operator*(ColorRGB color, float rate)
{
	return ColorRGB(color *= rate);
}

const ColorRGBA operator*(ColorRGBA color, float rate)
{
	return ColorRGBA(color *= rate);
}
