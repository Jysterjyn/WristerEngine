#include "IFadeManager.h"
#include "FadeManager.h"

void FadeManager::Initialize()
{
	sprite = spMan->Create("white1x1.png");
	sprite->size = WE::WIN_SIZE;
	sprite->color = WE::ColorRGBA::Black();
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
}

void FadeManager::Draw()
{
	sprite->Draw();
}
