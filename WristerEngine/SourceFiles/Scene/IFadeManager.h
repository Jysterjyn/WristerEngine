#pragma once
#include "SpriteManager.h"

namespace WristerEngine
{
	// 画面のフェード演出
	class IFadeManager : public _2D::SpriteUtility
	{
	protected:
		bool isFade = false;
		bool isChange = false;

	public:
		virtual ~IFadeManager() = default;
		// 初期化
		virtual void Initialize() = 0;
		// 更新
		virtual void Update() = 0;
		// 描画
		virtual void Draw() = 0;
		// フェードをオンにする
		void FadeScene() { isFade = true; }
		// getter
		bool IsChange() const { return isChange; }
		bool IsFade() const { return isFade; }
	};
}