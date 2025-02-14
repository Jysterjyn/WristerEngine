#include "FadeManager.h"
#include "WindowsAPI.h"
using namespace WristerEngine;

void FadeManager::Initialize()
{
	sprite = _2D::Sprite::Create("white1x1.png");
	sprite->size = WIN_SIZE;
	sprite->color = ColorRGBA::Black();
}

void FadeManager::Update()
{
	const float ALPHA_MAX = 1.0f;
	
	if (!isFade) { return; }
	isChange = false;
	alpha += fadePerFlame;
	if (alpha >= ALPHA_MAX)
	{
		alpha = ALPHA_MAX;
		fadePerFlame = -fadePerFlame;
		isChange = true;
	}
	if (alpha <= 0)
	{
		alpha = 0;
		fadePerFlame = -fadePerFlame;
		isFade = false;
	}
	sprite->color.a = alpha;
	sprite->Update();
}