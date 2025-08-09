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
		bool isFinish = false;

	protected:
		uPtr<Parameter> param;

	public:
		virtual ~Behavior() = default;
		virtual void Initialize() = 0;
		virtual void Update() = 0;
		void Finish() { isFinish = true; }
		bool IsFinish() const { return isFinish; }
		static void Inherit(Behavior* b1, Behavior* b2);
		const Parameter* GetParam() const { return param.get(); }
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
		const Behavior::Parameter* GetParam() const;
	};
}