#include "BaseMapChip.h"
using namespace WristerEngine::_2D;

void BaseMapChip::Initialize(const Vector2& leftTopPos_, const Vector2& chipSize_, const Vector2& chipAnchor_)
{
	leftTopPos = leftTopPos_;
	chipSize = chipSize_;
	chipAnchor = chipAnchor_;
}

void BaseMapChip::AddSprite(const std::string& fileName, const Vector2& chipPos, float splitNum, const Vector2& drawSize)
{
	std::unique_ptr<Sprite> newSprite = Sprite::Create(fileName);
	if (drawSize.Length() != 0) { newSprite->size = drawSize; }
	newSprite->position = leftTopPos;
	newSprite->position.x += chipPos.x * chipSize.x;
	newSprite->position.y += chipPos.y * chipSize.y;
	newSprite->anchorPoint = chipAnchor;
	VirtualInitialize(newSprite.get(), splitNum);
	newSprite->Update();
	sprites.push_back(std::move(newSprite));
}

void BaseMapChip::Draw()
{
	for (auto& s : sprites) { s->Draw(); }
}
