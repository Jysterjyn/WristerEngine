#include <string>
#include <memory>
#include <list>
#include <WristerEngineUtility.h>
#include <Timer.h>

namespace WristerEngine
{
	// 受け継ぐパラメータ
	struct BehaviorParameter
	{
	private:
		bool isDestroy = false;

	public:
		std::string name;

		virtual ~BehaviorParameter() = default;
		void Destroy() { isDestroy = true; }
		bool IsDestroy() const { return isDestroy; }
	};

	// ビヘイビア単体
	class Behavior
	{
	private:
		bool isFinish = false;

	protected:
		uPtr<BehaviorParameter> param;

	public:
		virtual ~Behavior() = default;
		virtual void Initialize() = 0;
		virtual void Update() = 0;
		void Finish() { isFinish = true; }
		bool IsFinish() const { return isFinish; }
		static void Inherit(Behavior* b1, Behavior* b2);
		BehaviorParameter* GetParam() { return param.get(); }
	};

	// ビヘイビアとクールタイム
	using BehaviorPair = std::pair<uPtr<Behavior>, int>;

	// ビヘイビアの一連の流れ
	class BehaviorPattern
	{
	private:
		std::list<BehaviorPair> behaviorList;
		bool isCoolTime = false;
		FrameTimer coolTime;

	public:
		void Add(BehaviorPair& behavior);
		void Update();
		BehaviorParameter* GetParam();
	};
}