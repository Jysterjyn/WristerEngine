#include <Behavior.h>

void Inherit(std::unique_ptr<Behavior>& b1, std::unique_ptr<Behavior>& b2)
{
	// もしかしたらエラー出る？
	b2->param = b1->param;
	b1.release();
}