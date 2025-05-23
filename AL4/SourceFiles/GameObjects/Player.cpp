#include "Player.h"

void Player::Initialize()
{
	WristerEngine::_3D::ModelManager* modelManager = WristerEngine::_3D::ModelManager::GetInstance();
	objects = modelManager->CreateGroup("Player", true);
	rootPos.translation.y = 2;

	for (auto o : objects)
	{
		o.second->material.ambient = { 0,0,0 };
		o.second->transform.parent = &rootPos;
	}

	objects["chest"]->transform.translation.y = 1.0f;
	objects["head"]->transform.translation.y = 3.5f;
	objects["handLeft"]->transform.translation = { 1.4f,1.7f,0 };
	objects["handRight"]->transform.translation = { -1.4f,1.7f,0 };
	objects["footLeft"]->transform.translation = { -0.6f,-0.3f,0 };
	objects["footRight"]->transform.translation = { 0.6f,-0.3f,0 };
}

void Player::Update()
{
	rootPos.Update();
}

void Player::Draw()
{
}
