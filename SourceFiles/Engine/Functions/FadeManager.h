#pragma once
#include "Sprite.h"

namespace WristerEngine
{
	// 画面のフェード演出
	class FadeManager
	{
	private:
		const int FADE_TIME = 30;
		float fadePerFlame = 1.0f / FADE_TIME;
		std::unique_ptr<_2D::Sprite> sprite;
		bool isFade = 0;
		float alpha = 0;
		bool isChange = false;

	public:
		// 初期化
		void Initialize();
		// 更新
		void Update();
		// 描画
		void Draw() { sprite->Draw(); }
		// フェードをオンにする
		void FadeScene() { isFade = true; }
		// getter
		bool IsChange() { return isChange; }
		bool IsFade() { return isFade; }
	};
}