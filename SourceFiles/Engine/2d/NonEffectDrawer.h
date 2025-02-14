#pragma once
#include "Sprite.h"

namespace WristerEngine
{
	// ポストエフェクトの影響を受けない描画
	class NonEffectDrawer
	{
		static std::list<_2D::Sprite*> sprites;

	public:
		static void AddSprite(_2D::Sprite* sprite)
		{
			sprites.push_back(sprite);
		}
		static void Clear() { sprites.clear(); }
		static void Draw()
		{
			sprites.remove_if([](_2D::Sprite* s)
				{
					return (std::abs(s->position.x) >= 10000 || std::abs(s->position.x) <= 0.0001f);
				});

			for (auto s : sprites)
			{
				s->Draw();
			}
		}
	};
}