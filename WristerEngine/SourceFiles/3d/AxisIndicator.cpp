#include "AxisIndicator.h"

void WristerEngine::AxisIndicator::Initialize()
{
	obj = modelManager->Create("axis");
	camera.Initialize();
	modelManager->AddCamera("axis", &camera);
}

void WristerEngine::AxisIndicator::Update()
{

}

void WristerEngine::AxisIndicator::Draw()
{
}
