#include "Ground.h"
#include <ModelManager.h>
#include <CollisionInfo.h>

void Ground::Initialize(const std::string& modelName, const Vector3& scale)
{
	WE::Collider::Initialize("Ground");

	object = mm->Create(modelName);
	object->transform.scale = scale;
	object->transform.translation.y = -2.0f;
	object->material.textures[(size_t)WE::_3D::TexType::Main].tiling = { scale.x,scale.z };
	object->material.ChangeTexture((size_t)WE::_3D::TexType::Main, "./dirt.jpg");
	object->material.ambient = { 1,1,1 };

	WE::PlaneCollider* collider = static_cast<WE::PlaneCollider*>(AddCollider(WE::CollisionShapeType::Plane));
	collider->SetAttribute(ChangeVal(CollisionAttribute::Plane));
	collider->SetDistance(-1.0f);
	group->SetAttribute(ChangeVal(CollisionAttribute::Plane));
}

void Sphere::Initialize()
{
	WE::Collider::Initialize("Sphere");

	object = mm->Create("TestSphere", true);
	object->material.ambient = { 0.1f,0.1f,0.1f };

	WE::SphereCollider* collider = static_cast<WE::SphereCollider*>(AddCollider(WE::CollisionShapeType::Sphere));
	collider->SetAttribute(ChangeVal(CollisionAttribute::Sphere));
	collider->SetTransform(&object->transform);

	group->SetAttribute(ChangeVal(CollisionAttribute::Sphere));
}

void Sphere::Update()
{
	object->transform.translation.y += input->Move(WE::Key::W, WE::Key::S, 0.1f);
	object->material.ambient = { 0.1f,0.1f,0.1f };
}