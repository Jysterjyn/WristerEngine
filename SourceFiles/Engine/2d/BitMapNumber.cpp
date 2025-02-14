#include "BitMapNumber.h"
using namespace WristerEngine;
using namespace _2D;

void BitMapProp::SetVal(const std::string& fileName_, Vector2 rectSize_, Vector2 size_, Vector2 pos_, int digit_)
{
	fileName = fileName_;
	rectSize = rectSize_;
	size = size_;
	pos = pos_;
	digit = digit_;
}

void BitMapNumber::UpdateSprite(size_t index)
{
	sprites[index]->size = bitMapProp.size;

	// 配置
	Vector2 sprpos = bitMapProp.pos;
	sprpos.x += bitMapProp.size.x * index;
	sprites[index]->position = sprpos;
}

void BitMapNumber::Initialize()
{
	for (size_t i = 0; i < bitMapProp.digit; i++)
	{
		// 数字のスプライト
		sprites.push_back(Sprite::Create(bitMapProp.fileName));
		if (allSpriteSize.x == 0) { allSpriteSize = sprites[i]->textureSize; }
		sprites[i]->textureSize = bitMapProp.rectSize;
		UpdateSprite(i);
	}
}

void BitMapNumber::Update(int number)
{
	std::vector<int> numchar(sprites.size());
	bool isCountStop = log10(number) >= (double)sprites.size();

	// 桁の抽出
	for (int i = (int)sprites.size() - 1; i >= 0; i--)
	{
		if (!isCountStop)
		{
			numchar[i] = number % 10;
			number /= 10;
		}
		else { numchar[i] = 9; }
	}

	for (size_t i = 0; i < sprites.size(); i++)
	{
		sprites[i]->textureLeftTop.x = numchar[i] * sprites[i]->textureSize.x;
		sprites[i]->textureLeftTop += bitMapProp.texLTOffset;
		UpdateSprite(i);
		sprites[i]->color = bitMapProp.color;
		sprites[i]->anchorPoint = bitMapProp.anchorPoint;
		sprites[i]->Update();
	}
}

void BitMapNumber::Draw()
{
	for (auto& spr : sprites) { spr->Draw(); }
}