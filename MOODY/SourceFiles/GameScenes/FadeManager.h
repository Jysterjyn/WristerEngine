#pragma once
#include "IFadeManager.h"

class FadeManager : public WE::IFadeManager
{
	const int FADE_TIME = 30;
	float fadePerFlame = 1.0f / FADE_TIME;
	WE::_2D::Sprite* sprite = nullptr;
	float alpha = 0;

	// IFadeManager を介して継承されました
	void Initialize() override;

	void Update() override;

	void Draw() override;
};
