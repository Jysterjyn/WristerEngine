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

void Triangle::Initialize()
{
	WE::Collider::Initialize("Triangle");

	object = mm->Create("TestTriangle");
	object->transform.scale *= 5.0f;
	object->transform.translation.y = -1.0f;
	object->material.textures[(size_t)WE::_3D::TexType::Main].tiling = {
		object->transform.scale.x,object->transform.scale.z };

	const WE::_3D::Mesh* mesh = object->GetMesh();
	auto& vertices = mesh->GetVertices();

	for (size_t i = 0; i < p.size(); i++)
	{
		p[i] = vertices[i].pos;
	}

	collider = static_cast<WE::TriangleCollider*>(AddCollider(WE::CollisionShapeType::Triangle));
	collider->SetAttribute(ChangeVal(CollisionAttribute::Triangle));
	collider->SetTransform(&object->transform);
	group->SetAttribute(ChangeVal(CollisionAttribute::Triangle));
}

void Triangle::Update()
{
	BaseObject::Update();
	if (inputIndex == 1)
	{
		object->transform.rotation.x += input->Move(WE::Key::W, WE::Key::S, Angle(1));
		object->transform.rotation.z += input->Move(WE::Key::A, WE::Key::D, Angle(1));
		WE::ImGuiManager::PrintVector("GroundNormal", collider->GetNormal());

		std::array<Vector3, 3> v;
		for (size_t i = 0; i < p.size(); i++)
		{
			v[i] = p[i] * object->transform.matWorld;
		}
		collider->SetVertices(v);
		WE::ImGuiManager::PrintVector("TriangleVertex0", v[0]);
		WE::ImGuiManager::PrintVector("TriangleVertex1", v[1]);
		WE::ImGuiManager::PrintVector("TriangleVertex2", v[2]);
	}
}
