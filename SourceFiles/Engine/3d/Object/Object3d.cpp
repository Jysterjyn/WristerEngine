#include "Object3d.h"
#include "ModelManager.h"
using namespace WristerEngine;
using namespace _3D;

void Object3d::Initialize(Mesh* mesh_)
{
	mesh = mesh_;
	material.Load(mesh_);
	worldTransform = std::make_unique<Transform>();
	worldTransform->Initialize();
}

void Object3d::Update()
{
	if (isInvisible) { return; }
	material.Update();
	worldTransform->Update();
}

void Object3d::Draw()
{
	if (isInvisible) { return; }
	worldTransform->Draw();
	material.Draw();
	mesh->Draw();
}