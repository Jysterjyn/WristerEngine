#include "BaseCharacter.h"
#include <ModelManager.h>

void BaseCharacter::Initialize(const std::string& modelGroupName)
{
	WE::_3D::ModelManager* modelManager = WristerEngine::_3D::ModelManager::GetInstance();
	objects = modelManager->CreateGroup(modelGroupName, true);
	rootPos.Initialize();
}

void BaseCharacter::Update()
{
	rootPos.Update();
	SetCenterPosition(rootPos.GetWorldPosition());
	debugObject->transform = rootPos;
	debugObject->isInvisible = !collisionManager->IsPrint();
}