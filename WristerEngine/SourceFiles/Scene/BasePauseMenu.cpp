#include "BasePauseMenu.h"
using namespace WristerEngine;

void BasePauseMenu::Update()
{
}

void BasePauseMenu::Draw()
{
	if (!isPause) { return; }
	for (auto& s : sprites) { s.second->Draw(); }
}