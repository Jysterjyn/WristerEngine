#include "Player.h"

void Player::Initialize()
{
	WristerEngine::_3D::ModelManager* modelManager = WristerEngine::_3D::ModelManager::GetInstance();
	obj = modelManager->Create("cube");
}

void Player::Update()
{
}

void Player::Draw()
{
}
