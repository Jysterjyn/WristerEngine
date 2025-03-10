#include "AxisIndicator.h"

void WristerEngine::AxisIndicator::Initialize()
{
	obj = modelManager->Create("axis");
	camera.Initialize();
	modelManager->AddCamera(&camera);
}

void WristerEngine::AxisIndicator::Update()
{

}

void WristerEngine::AxisIndicator::Draw()
{
}
