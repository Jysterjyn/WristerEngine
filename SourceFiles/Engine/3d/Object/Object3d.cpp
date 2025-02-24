#include "Object3d.h"
#include "ModelManager.h"
using namespace WristerEngine;
using namespace _3D;

void Object3d::Initialize(Mesh* mesh_)
{
	mesh = mesh_;
	material.Load(mesh_);
	transform = std::make_unique<Transform>();
	transform->Initialize();
}

void Object3d::Update()
{
	if (isInvisible) { return; }
	material.Update();
	transform->Update();
}

void Object3d::Draw()
{
	if (isInvisible) { return; }
	transform->Draw();
	material.Draw();
	mesh->Draw();
}