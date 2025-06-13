#include "Collider.h"
#include "CollisionManager.h"
#include <cassert>
using namespace WE;

//float IncludeCollider::includeRadius = 0.1f;

//_2D::Collider::~Collider()
//{
//	colliders.clear();
//	collisionManager->PopCollider(this);
//}
//
//void PolygonCollider::SetVertices()
//{
//	Vector3 objPos = pTransform->translation;
//	Vector3 objRad = pTransform->scale;
//	vertices.clear();
//	vertices.push_back(objPos + Vector3(-objRad.x, objRad.y, -objRad.z));
//	vertices.push_back(objPos + Vector3(objRad.x, objRad.y, -objRad.z));
//	vertices.push_back(objPos + Vector3(objRad.x, -objRad.y, -objRad.z));
//	vertices.push_back(objPos - objRad);
//}
//
//void PolygonCollider::ComputeNormal()
//{
//	assert(vertices.size() >= 3);
//	// 法線の計算
//	Vector3 vec1 = vertices[2] - vertices[0];
//	Vector3 vec2 = vertices[1] - vertices[0];
//	normal = Normalize(Cross(vec1, vec2));
//}
//
//void PolygonCollider::ToPlaneCollider(PlaneCollider* planeCollider)
//{
//	planeCollider->SetDistance(distance);
//	planeCollider->SetRotation(pTransform->rotation);
//	planeCollider->SetBaseNormal(baseNormal);
//}
//
//void PolygonCollider::UpdateVertices()
//{
//	for (Vector3& vertex : vertices) { vertex *= pTransform->matWorld; }
//}

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

void _2D::Base2DCollider::Initialize(Sprite* transform_, CollisionShapeType shapeType_, const std::string& colliderName_)
{
	transform = transform_;
	shapeType = shapeType_;
	colliderName = colliderName_;
}

std::map<std::string, Vector2> _2D::BoxCollider::GetVertex() const
{
	std::map<std::string, Vector2> ans;
	ans["LT"] = ans["RB"] = ans["LB"] = ans["RT"] = transform->position;
	// 差分
	Vector2 ltSub = Vector2(transform->size.x * transform->anchorPoint.x, transform->size.y * transform->anchorPoint.y);
	Vector2 rbSub = Vector2(transform->size.x * (1.0f - transform->anchorPoint.x), transform->size.y * (1.0f - transform->anchorPoint.y));
	if (transform->isFlipX) { ltSub.x = -ltSub.x; rbSub.x = -rbSub.x; }
	if (transform->isFlipY) { ltSub.y = -ltSub.y; rbSub.y = -rbSub.y; }
	ans["LT"] -= ltSub;
	ans["LB"] += ltSub;
	ans["RT"] -= rbSub;
	ans["RB"] += rbSub;
	return ans;
}

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

BaseCollider* ColliderGroup::AddCollider(CollisionShapeType shapeType, Collider* owner)
{
	std::unique_ptr<BaseCollider> newCollider;

	switch (shapeType)
	{
	case CollisionShapeType::Sphere:
		newCollider = std::make_unique<SphereCollider>();
		break;
	case CollisionShapeType::Box:
		break;
	case CollisionShapeType::IncludeBox:
		break;
	case CollisionShapeType::Plane:
		break;
	case CollisionShapeType::Ray:
		break;
	case CollisionShapeType::Mesh:
		break;
	default:
		break;
	}

	newCollider->SetOwner(owner);
	colliders.push_back(std::move(newCollider));
	return colliders.back().get();
}

void ColliderGroup::Update()
{
	// コライダーの削除
	colliders.remove_if([](std::unique_ptr<BaseCollider>& collider)
		{ return collider.get()->IsDestroy(); });

	for (auto& collider : colliders) { collider->Update(); }
	collisionPair.clear();
}

void ColliderGroup::AddCollisionPair(BaseCollider* colliderA, BaseCollider* colliderB)
{
	collisionPair.push_back({ colliderA,colliderB });
}

void Collider::Initialize(const std::string& groupName)
{
	group = CollisionManager::GetInstance()->GetGroup(groupName);
}
