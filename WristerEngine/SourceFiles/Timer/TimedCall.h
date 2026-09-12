#pragma once
#include <functional>
#include <Timer.h>
#include <WristerEngineUtility.h>

namespace WristerEngine
{	
	// 時限発動
	class TimedCall : public ListObject
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
		TimedCall(std::function<void(void)>callfunc, uint32_t time);
		void Initialize() override {}
		// 更新
		void Update() override;
		// 完了ならtrueを返す
		bool Remove() const override { return isFinished; }
	};
}