#include "Collider.h"
#include "CollisionManager.h"
#include <cassert>
#include <imgui.h>
using namespace WE;

float IncludeCollider::includeRadius = 0.1f;
uint32_t BaseCollider::nextSerialNumber = 0;
uint32_t Collider::nextSerialNumber = 0;

//_2D::Collider::~Collider()
//{
//	colliders.clear();
//	collisionManager->PopCollider(this);
//}
//
//void MeshCollider::ConstructTriangles(ModelManager* model)
//{
//	triangles.clear();
//
//	const Mesh& mesh = model->GetMesh();
//
//	int start = 0;
//
//	const std::vector<Mesh::VertexData>& vertices = mesh.GetVertices();
//	const std::vector<unsigned short>& indices = mesh.GetIndices();
//
//	size_t triangleNum = indices.size() / 3;
//
//	triangles.resize(triangles.size() + triangleNum);
//
//	for (int i = 0; i < triangleNum; i++) {
//
//		PolygonCollider& tri = triangles[start + i];
//		int idx0 = indices[i * 3 + 0];
//		int idx1 = indices[i * 3 + 1];
//		int idx2 = indices[i * 3 + 2];
//
//		tri.AddVertices(vertices[idx0].pos);
//		tri.AddVertices(vertices[idx1].pos);
//		tri.AddVertices(vertices[idx2].pos);
//
//		tri.ComputeNormal();
//	}
//}
//
//void _2D::Base2DCollider::Initialize(Sprite* transform_, CollisionShapeType shapeType_, const std::string& colliderName_)
//{
//	transform = transform_;
//	shapeType = shapeType_;
//	colliderName = colliderName_;
//}
//
//std::map<std::string, Vector2> _2D::BoxCollider::GetVertex() const
//{
//	std::map<std::string, Vector2> ans;
//	ans["LT"] = ans["RB"] = ans["LB"] = ans["RT"] = transform->position;
//	// 差分
//	Vector2 ltSub = Vector2(transform->size.x * transform->anchorPoint.x, transform->size.y * transform->anchorPoint.y);
//	Vector2 rbSub = Vector2(transform->size.x * (1.0f - transform->anchorPoint.x), transform->size.y * (1.0f - transform->anchorPoint.y));
//	if (transform->isFlipX) { ltSub.x = -ltSub.x; rbSub.x = -rbSub.x; }
//	if (transform->isFlipY) { ltSub.y = -ltSub.y; rbSub.y = -rbSub.y; }
//	ans["LT"] -= ltSub;
//	ans["LB"] += ltSub;
//	ans["RT"] -= rbSub;
//	ans["RB"] += rbSub;
//	return ans;
//}
//
//void _2D::Collider::AddCollider(Sprite* transform, CollisionShapeType shapeType, const std::string& colliderName, const Option* option)
//{
//	std::unique_ptr<Base2DCollider> newCollider;
//	switch (shapeType)
//	{
//	case _2D::CollisionShapeType::Box:
//		newCollider = std::make_unique<_2D::BoxCollider>();
//		break;
//	case _2D::CollisionShapeType::TwoRay:
//		assert(option);
//		newCollider = std::make_unique<TwoRayCollider>(option->fov);
//		break;
//	default:
//		break;
//	}
//
//	newCollider->Initialize(transform, shapeType, colliderName);
//	colliders.push_back(move(newCollider));
//}
//
//void _2D::Collider::DeleteCollider(const std::string& colliderName)
//{
//	colliders.remove_if([&](const std::unique_ptr<Base2DCollider>& collider) { return collider->GetColliderName() == colliderName; });
//}
//
//void _2D::Collider::AddCollisionPair(size_t myIndex, size_t youIndex)
//{
//	collisionPair[myIndex].push_back(youIndex);
//}
//
//void _2D::Collider::DeletePair()
//{
//	for (auto& pair : collisionPair)
//	{
//		pair.second.clear();
//	}
//	collisionPair.clear();
//}
//
//const std::string _2D::Collider::GetColliderName(size_t index) const
//{
//	if (colliders.size() <= index) { return "Null"; }
//	auto itr = colliders.begin();
//	for (size_t i = 0; i < index; i++)
//	{
//		itr++;
//	}
//	return itr->get()->GetColliderName();
//}

CollisionPair::CollisionPair(BaseCollider* my_, BaseCollider* other_,
	const std::optional<Vector3>& inter_, std::optional<float> distance_)
{
	my = my_; other = other_; inter = inter_; distance = distance_;
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

void Collider::Initialize(const std::string& groupName)
{
	group = CollisionManager::GetInstance()->AddGroup(groupName);
	group->AddOwner(this);
}

BaseCollider* Collider::AddCollider(CollisionShapeType shapeType)
{
	std::unique_ptr<BaseCollider> newCollider;

	switch (shapeType)
	{
	case CollisionShapeType::Sphere:
		newCollider = std::make_unique<SphereCollider>();
		break;
	case CollisionShapeType::Box:
		newCollider = std::make_unique<BoxCollider>();
		break;
	case CollisionShapeType::IncludeBox:
		newCollider = std::make_unique<IncludeCollider>();
		break;
	case CollisionShapeType::Plane:
		newCollider = std::make_unique<PlaneCollider>();
		break;
	case CollisionShapeType::Triangle:
		newCollider = std::make_unique<TriangleCollider>();
		break;
	case CollisionShapeType::Ray:
		newCollider = std::make_unique<RayCollider>();
		break;
	case CollisionShapeType::Mesh:
		newCollider = std::make_unique<MeshCollider>();
		break;
	}

	newCollider->SetOwner(this);
	return group->AddCollider(std::move(newCollider));
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

void WristerEngine::TriangleCollider::ComputeNormal()
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

void WristerEngine::MeshCollider::Update()
{
	assert(pTransform);
	invMatWorld = Inverse(pTransform->matWorld);
}

void WristerEngine::MeshCollider::ConstructTriangles(const _3D::Mesh* mesh)
{
	triangles.clear();

	const std::vector<_3D::Mesh::VertexData>& vertices = mesh->GetVertices();
	const std::vector<unsigned short>& indices = mesh->GetIndices();

	size_t triangleNum = indices.size() / 3;

	for (size_t i = 0; i < triangleNum; i++)
	{
		std::unique_ptr<TriangleCollider> tri = std::make_unique<TriangleCollider>(true);
		int idx0 = indices[i * 3 + 0];
		int idx1 = indices[i * 3 + 1];
		int idx2 = indices[i * 3 + 2];

		std::array<Vector3, 3> v;
		v[0] = { vertices[idx0].pos.x,vertices[idx0].pos.y,vertices[idx0].pos.z, };
		v[1] = { vertices[idx1].pos.x,vertices[idx1].pos.y,vertices[idx1].pos.z, };
		v[2] = { vertices[idx2].pos.x,vertices[idx2].pos.y,vertices[idx2].pos.z, };

		tri->SetVertices(v);
		tri->ComputeNormal();
		triangles.push_back(std::move(tri));
	}
}
