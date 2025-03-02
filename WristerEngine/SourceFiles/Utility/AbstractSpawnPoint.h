#pragma once
#include "Timer.h"

namespace WristerEngine
{
	// スポーンオブジェクト基底クラス
	class AbstractSpawnPoint
	{
	protected:
		FrameTimer spawnTimer;

	public:
		// 仮想デストラクタ
		virtual ~AbstractSpawnPoint() = default;
		// 更新
		virtual void Update() = 0;
	};
}