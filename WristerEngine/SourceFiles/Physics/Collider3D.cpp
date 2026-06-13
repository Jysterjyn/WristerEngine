#include "Collider3D.h"
#include "CollisionManager.h"
#include <cassert>
#include <imgui.h>
using namespace WE;
using namespace _3D;

float IncludeCollider::includeRadius = 0.1f;
uint32_t BaseCollider::nextSerialNumber = 0;
uint32_t Collider::nextSerialNumber = 0;

CollisionPair::CollisionPair(BaseCollider* my_, BaseCollider* other_, const HitInfo& hitInfo)
{
	my = my_; other = other_; inter = hitInfo.inter; distance = hitInfo.distance; reject = hitInfo.reject;
}

bool CollisionPair::Check(const CollisionPair& p1, const CollisionPair& p2)
{
	if (p1.my->GetSerialNumber() == p2.my->GetSerialNumber() &&
		p1.other->GetSerialNumber() == p2.other->GetSerialNumber())
	{
		return true;
	}

	if (p1.my->GetSerialNumber() == p2.other->GetSerialNumber() &&
		p1.other->GetSerialNumber() == p2.my->GetSerialNumber())
	{
		return true;
	}
	return false;
}

BaseCollider* ColliderGroup::AddCollider(std::unique_ptr<BaseCollider> newCollider)
{
	colliders.push_back(std::move(newCollider));
	return colliders.back().get();
}

void ColliderGroup::Update()
{
	// コライダーの削除
	colliders.remove_if([](std::unique_ptr<BaseCollider>& collider)
		{ return collider.get()->IsDestroy(); });

	for (auto& collider : colliders) { collider->Update(); }
	collisionPairs.clear();
	enterPairs.clear();
	exitPairs.clear();
}

void ColliderGroup::AddCollisionPair(const CollisionPair& pair)
{
	collisionPairs.push_back(pair);
}

void ColliderGroup::CallCollision()
{
	std::map<uint32_t, uint8_t> calledCollision;
	for (auto& pair : collisionPairs)
	{
		Collider* owner = pair.my->GetOwner();
		if (calledCollision.contains(owner->GetSerialNumber())) { continue; }
		owner->OnCollision();
		calledCollision[owner->GetSerialNumber()];
	}

	for (auto& pair : collisionPairs)
	{
		bool isNotEnter = false;
		for (auto& pairPre : collisionPairsPre)
		{
			if (CollisionPair::Check(pair, pairPre)) { isNotEnter = true; break; }
		}
		if (isNotEnter) { continue; }
		enterPairs.push_back(pair);
	}
	calledCollision.clear();
	for (auto& pair : enterPairs)
	{
		Collider* owner = pair.my->GetOwner();
		if (calledCollision.contains(owner->GetSerialNumber())) { continue; }
		owner->OnCollisionEnter();
		calledCollision[owner->GetSerialNumber()];
	}

	for (auto& pair : collisionPairsPre)
	{
		bool isNotEnter = false;
		for (auto& pairPre : collisionPairs)
		{
			if (CollisionPair::Check(pair, pairPre)) { isNotEnter = true; break; }
		}
		if (isNotEnter) { continue; }
		exitPairs.push_back(pair);
	}
	calledCollision.clear();
	for (auto& pair : exitPairs)
	{
		Collider* owner = pair.my->GetOwner();
		if (calledCollision.contains(owner->GetSerialNumber())) { continue; }
		owner->OnCollisionExit();
		calledCollision[owner->GetSerialNumber()];
	}

	collisionPairsPre = collisionPairs;
}

ColliderGroup::~ColliderGroup()
{
	colliders.clear();
	for (auto* owner : owners) { owner->DeleteGroup(); }
}

void Collider::Initialize(const std::string& groupName, const std::optional<CollisionInfo>& info)
{
	group = CollisionManager::GetInstance()->AddGroup(groupName);
	group->AddOwner(this);
	if (!info) { return; }
	group->SetAttribute(info->GetAttribute());
	group->SetMask(info->GetMask());
}

Collider::~Collider()
{
	if (!group) { return; }
	for (auto& collider : *group->GetColliders())
	{
		if (collider->GetOwner() == this) { collider->Destroy(); }
	}
}

PlaneCollider::PlaneCollider(const TriangleCollider& triangle) : BaseCollider(true)
{
	shapeType = CollisionShapeType::Plane;
	normal = triangle.GetNormal();
	distance = Dot(triangle.GetNormal(), triangle.GetVertices()[0]);
}

void PlaneCollider::Update()
{
	if (!pTransform) { return; }
	normal = baseNormal * Matrix4::Rotate(pTransform->rotation);
	distance = Dot(pTransform->GetWorldPosition(), normal);
}

void TriangleCollider::Update()
{
	if (!pTransform) { return; }
	for (size_t i = 0; i < initV.size(); i++)
	{
		vertices[i] = initV[i] * pTransform->matWorld;
	}

	normal = baseNormal * Matrix4::Rotate(pTransform->rotation);
}

void TriangleCollider::ComputeNormal()
{
	Vector3 p0_p1 = vertices[1] - vertices[0];
	Vector3 p0_p2 = vertices[2] - vertices[0];
	SetNormal(Cross(p0_p1, p0_p2));
}

void RayCollider::Update()
{
	if (!pTransform) { return; }
	start = pTransform->GetWorldPosition();
	dir = baseDir * Matrix4::Rotate(pTransform->rotation);
}

void MeshCollider::Update()
{
	assert(pTransform);
	invMatWorld = Inverse(pTransform->matWorld);
}

void MeshCollider::ConstructTriangles(const _3D::Mesh* mesh)
{
	triangles.clear();

	const std::vector<_3D::Mesh::VertexData>& vertices = mesh->GetVertices();
	const std::vector<unsigned short>& indices = mesh->GetIndices();

	size_t triangleNum = indices.size() / 3;

	for (size_t i = 0; i < triangleNum; i++)
	{
		std::array<int, 3> idx{};
		std::array<Vector3, 3> v;
		for (size_t j = 0; j < idx.size(); j++)
		{
			idx[j] = indices[i * 3 + j];
			v[j] = { vertices[idx[j]].pos.x,vertices[idx[j]].pos.y,vertices[idx[j]].pos.z, };
		}

		std::unique_ptr<TriangleCollider> tri = std::make_unique<TriangleCollider>(true);
		tri->SetVertices(v);
		tri->ComputeNormal();
		triangles.push_back(std::move(tri));
	}
}