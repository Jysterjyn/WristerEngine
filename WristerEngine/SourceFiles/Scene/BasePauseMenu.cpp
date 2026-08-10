#include "BasePauseMenu.h"
using namespace WE;

void BasePauseMenu::Draw()
{
	if (!isPause) { return; }
	for (auto& s : sprites) { s.second->Draw(); }
}