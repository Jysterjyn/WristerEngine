#include "Ground.h"
#include <ModelManager.h>
#include <CollisionInfo.h>
#include <ParticleManager.h>
#include <DiffuseParticle.h>
#include <ImGuiManager.h>
#include <imgui.h>

uint32_t BaseObject::inputIndex = 0;

void Ground::Initialize(const std::string& modelName, const Vector3& scale)
{
	WE::Collider::Initialize("Ground");

	object = mm->Create(modelName);
	object->transform.scale = scale;
	object->transform.translation.y = -1.0f;
	object->material.textures[(size_t)WE::_3D::TexType::Main].tiling = { scale.x,scale.z };
	object->material.ChangeTexture((size_t)WE::_3D::TexType::Main, "./dirt.jpg");
	object->material.ambient = { 1,1,1 };

	collider = static_cast<WE::PlaneCollider*>(AddCollider(WE::CollisionShapeType::Plane));
	collider->SetAttribute(ChangeVal(CollisionAttribute::Plane));
	collider->SetTransform(&object->transform);
	group->SetAttribute(ChangeVal(CollisionAttribute::Plane));
}

void Ground::Update()
{
	BaseObject::Update();
	if (inputIndex == 1)
	{
		object->transform.rotation.x += input->Move(WE::Key::W, WE::Key::S, Angle(1));
		object->transform.rotation.z += input->Move(WE::Key::A, WE::Key::D, Angle(1));
		ImGui::Text("distance = %f", collider->GetDistance());
		WE::ImGuiManager::PrintVector("GroundNormal", collider->GetNormal());
	}
}

void Ground::OnCollision()
{
	object->material.ambient = { 1,0,0 };
	std::optional<Vector3> inter = GetCollisionPairs()[0].inter;
	if (inter)
	{
		WE::ParticleGroup* pGroup = WE::ParticleManager::GetParticleGroup(0);
		WE::DiffuseParticle::AddProp dprop;
		dprop.posOffset = inter.value();
		pGroup->Add(dprop);
	}
}

void Sphere::Initialize()
{
	WE::Collider::Initialize("Sphere");

	object = mm->Create("TestSphere", true);
	object->material.ambient = { 0.1f,0.1f,0.1f };

	collider = static_cast<WE::SphereCollider*>(AddCollider(WE::CollisionShapeType::Sphere));
	collider->SetAttribute(ChangeVal(CollisionAttribute::Sphere));
	collider->SetTransform(&object->transform);

	group->SetAttribute(ChangeVal(CollisionAttribute::Sphere));
}

void Sphere::Update()
{
	if (inputIndex == 0)
	{
		object->transform.translation.x += input->Move(WE::Key::D, WE::Key::A, 0.1f);
		object->transform.translation.y += input->Move(WE::Key::W, WE::Key::S, 0.1f);
		object->transform.translation.z += input->Move(WE::Key::Up, WE::Key::Down, 0.1f);
	
		WE::ImGuiManager::PrintVector("SpherePosition", collider->GetCenterPosition());
	}
	object->material.ambient = { 0.1f,0.1f,0.1f };
}