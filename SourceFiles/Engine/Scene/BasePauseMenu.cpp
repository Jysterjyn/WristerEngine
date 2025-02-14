#include "BasePauseMenu.h"
using namespace WristerEngine;

void BasePauseMenu::Update()
{
	if (!isPause) { return; }
	for (auto& s : sprites) { s.second->Update(); }
}

void BasePauseMenu::Draw()
{
	if (!isPause) { return; }
	for (auto& s : sprites) { s.second->Draw(); }
}