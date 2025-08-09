#include <string>
#include <memory>
#include <list>
#include <WristerEngineUtility.h>
#include <Timer.h>

namespace WristerEngine
{
	// ビヘイビア単体
	class Behavior
	{
	public:
		// 受け継ぐパラメータ
		struct Parameter
		{
			std::string name;

			virtual ~Parameter() = default;
		};

	private:
		Parameter param;
		bool isFinish = false;

	public:
		virtual ~Behavior() = default;
		virtual void Initialize() {}
		virtual void Update() {}
		bool IsFinish() const { return isFinish; }
		static void Inherit(uPtr<Behavior>& b1, uPtr<Behavior>& b2);
	};

	// ビヘイビアとクールタイム
	using BehaviorPair = std::pair<Behavior, int>;

	// ビヘイビアの一連の流れ
	class BehaviorPattern
	{
	private:
		uList<BehaviorPair> behaviorList;
		bool isCoolTime = false;
		FrameTimer coolTime;

	public:
		void Add(uPtr<BehaviorPair>& behavior);
		void Update();
	};
}