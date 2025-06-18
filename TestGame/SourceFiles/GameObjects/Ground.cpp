#include "Ground.h"
#include <ModelManager.h>
#include <CollisionInfo.h>
#include <ParticleManager.h>
#include <DiffuseParticle.h>
#include <ImGuiManager.h>
#include <imgui.h>
#include <PrimitiveDrawer.h>

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
	object->transform.rotation.x += input->Move(WE::Key::W, WE::Key::S, Angle(1));
	object->transform.rotation.z += input->Move(WE::Key::A, WE::Key::D, Angle(1));
	ImGui::Text("distance = %f", collider->GetDistance());
	WE::ImGuiManager::PrintVector("GroundNormal", collider->GetNormal());
}

void Ground::OnCollision()
{
	object->material.ambient = { 1,0,0 };
}

void Sphere::Initialize()
{
	WE::Collider::Initialize("Sphere");

	object = mm->Create("TestSphere", true);
	object->material.ambient = { 0.1f,0.1f,0.1f };

	object->transform.translation.y = 1.0f;

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
	}

	WE::ImGuiManager::PrintVector("SpherePosition", collider->GetCenterPosition());
	object->material.ambient = { 0.1f,0.1f,0.1f };
}

void Sphere::OnCollision()
{
	object->material.ambient = { 1,0,0 };
}

void Ray::Initialize()
{
	WE::Collider::Initialize("Ray");
	collider = static_cast<WE::RayCollider*>(AddCollider(WE::CollisionShapeType::Ray));
	collider->SetAttribute(ChangeVal(CollisionAttribute::Ray));
	group->SetAttribute(ChangeVal(CollisionAttribute::Ray));
	pos.y = 4.0f;
}

void Ray::Update()
{
	WE::ImGuiManager::SliderVector("RayPos", pos, -10.0f, 10.0f);
	WE::ImGuiManager::SliderVector("RayDir", dir, -1.0f, 1.0f);
	dir.Normalize();

	collider->SetDir(dir);
	collider->SetStartPos(pos);

	WE::_3D::PrimitiveDrawer* pDrawer = WE::_3D::PrimitiveDrawer::GetInstance();
	pDrawer->ClearLines();
	pDrawer->DrawLine3d(pos, pos + dir * 100.0f, WE::ColorRGB::Red());
	pDrawer->TransferVertices();
}

void Ray::OnCollision()
{
	auto pairs = group->GetCollisionPairs();
	std::sort(pairs.begin(), pairs.end(), [](const WE::CollisionPair& p1, const WE::CollisionPair& p2)
		{
			return p1.distance.value() < p2.distance.value();
		});

	if (!pairs[0].inter) { return; }
	WE::ParticleGroup* pGroup = WE::ParticleManager::GetParticleGroup(0);
	WE::DiffuseParticle::AddProp dprop;
	dprop.posOffset = pairs[0].inter.value();
	dprop.posRange = { 0,0,0 };
	dprop.velOffset = { 0,0,0 };
	dprop.velRange = { 0.05f,0.05f,0.05f };
	dprop.accOffset = { 0,0,0 };
	dprop.accRange = { 0,0,0 };
	pGroup->Add(dprop);
}

void Triangle::Initialize()
{
	WE::Collider::Initialize("Triangle");

	object = mm->Create("sword", true);
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

	collider = static_cast<WE::MeshCollider*>(AddCollider(WE::CollisionShapeType::Mesh));
	collider->SetAttribute(ChangeVal(CollisionAttribute::Triangle));
	collider->SetTransform(&object->transform);
	collider->ConstructTriangles(object->GetMesh());
	object->transform.rotation = { 0,0,Angle(90) };
	group->SetAttribute(ChangeVal(CollisionAttribute::Triangle));
}

void Triangle::Update()
{
	object->material.ambient = { 0.1f,0.1f,0.1f };
	object->transform.rotation.x += input->Move(WE::Key::W, WE::Key::S, Angle(1));
	object->transform.rotation.z += input->Move(WE::Key::A, WE::Key::D, Angle(1));
	object->transform.rotation.y += input->Move(WE::Key::Left, WE::Key::Right, Angle(1));
	WE::ImGuiManager::PrintVector("Mesh Rot", object->transform.rotation);
}
