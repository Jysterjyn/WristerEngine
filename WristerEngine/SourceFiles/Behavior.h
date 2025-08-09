#include <string>
#include <memory>

// ビヘイビア単体
class Behavior
{
public:
	// 受け継ぐパラメータ
	struct Parameter
	{
		std::string name;

		virtual ∼Parameter();
	}

private:
	Parameter param;

public:
	virtual ∼Behavior() = default;
	virtual void Initialize() {}
	virtual void Update() {}
	static void Inherit(std::unique_ptr<Behavior>& b1, std::unique_ptr<Behavior>& b2);
};

// ビヘイビアの一連の流れ
class BehaviorPattern
{

};

