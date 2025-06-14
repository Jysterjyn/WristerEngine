#pragma once
#include <functional>
#include <Timer.h>

namespace WristerEngine
{	// 時限発動
	class TimedCall
	{
	private:
		// コールバック
		std::function<void(void)> callfunc;
		// 残り時間
		FrameTimer timer;
		// 完了フラグ
		bool isFinished = false;

	public:
		// コンストラクタ
		TimedCall(std::function<void(void)>callfunc, uint32_t time);;
		// 更新
		void Update();
		// 完了ならtrueを返す
		bool IsFinished()const { return isFinished; }
	};
}