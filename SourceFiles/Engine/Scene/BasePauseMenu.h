#pragma once
#include "BaseScene.h"
#include "AbstractUIDrawer.h"
#include <map>

namespace WristerEngine
{
	class BasePauseMenu:public _2D::AbstractUIDrawer
	{
	protected:
		bool isPause = false;

	public:
		virtual ~BasePauseMenu() = default;
		virtual void Update() override;
		virtual void Draw() override;
		void Pause() { isPause = !isPause; }
		bool IsPause() const { return isPause; }
	};
}