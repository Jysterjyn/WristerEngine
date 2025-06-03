#include "LockOn.h"
#include <Input.h>

void LockOn::Initialize()
{
	lockOnMark = WE::_2D::Sprite::Create({ "Reticle.png" });
}

void LockOn::Update(const std::list<std::unique_ptr<Enemy>>& enemies)
{
	WE::Input* input = WE::Input::GetInstance();
	input->SetDeadZone(0, 1, 1);
	enemies;
	if (target)
	{

	}
	else
	{
		if (input->IsConnectGamePad() && input->IsTrigger(0, WE::JoyPad::X))
		{

		}
	}

	if (target)
	{

	}
}

void LockOn::Draw()
{
	if (target) { lockOnMark->Draw(); }
}
