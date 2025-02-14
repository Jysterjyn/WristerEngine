#include "Collider.h"
#include "CollisionManager.h"
#include <cassert>
using namespace WristerEngine;

float IncludeCollider::includeRadius = 0.1f;

BoxCollider::BoxCollider() { CollisionManager::PushCollider(this); }
BoxCollider::~BoxCollider() { CollisionManager::PopCollider(this); }
SphereCollider::SphereCollider() { CollisionManager::PushCollider(this); }
SphereCollider::~SphereCollider() { CollisionManager::PopCollider(this); }
RayCollider::RayCollider() { CollisionManager::PushCollider(this); }
RayCollider::~RayCollider() { CollisionManager::PopCollider(this); }
PolygonCollider::PolygonCollider() { CollisionManager::PushCollider(this); }
PolygonCollider::~PolygonCollider() { CollisionManager::PopCollider(this); }
PlaneCollider::PlaneCollider() { CollisionManager::PushCollider(this); }
PlaneCollider::~PlaneCollider() { CollisionManager::PopCollider(this); }
IncludeCollider::IncludeCollider() { CollisionManager::PushCollider(this); }
IncludeCollider::~IncludeCollider() { CollisionManager::PopCollider(this); }
WristerEngine::_2D::ColliderGroup::ColliderGroup() { CollisionManager::PushCollider(this); }

WristerEngine::_2D::ColliderGroup::~ColliderGroup()
{
	colliders.clear();
	CollisionManager::PopCollider(this);
}

void PolygonCollider::SetVertices()
{
	Vector3 objPos = worldTransform->translation;
	Vector3 objRad = worldTransform->scale;
	vertices.clear();
	vertices.push_back(objPos + Vector3(-objRad.x, objRad.y, -objRad.z));
	vertices.push_back(objPos + Vector3(objRad.x, objRad.y, -objRad.z));
	vertices.push_back(objPos + Vector3(objRad.x, -objRad.y, -objRad.z));
	vertices.push_back(objPos - objRad);
}

void PolygonCollider::ComputeNormal()
{
	assert(vertices.size() >= 3);
	// –@ü‚ÌŒvŽZ
	Vector3 vec1 = vertices[2] - vertices[0];
	Vector3 vec2 = vertices[1] - vertices[0];
	normal = Normalize(Cross(vec1, vec2));
}

void PolygonCollider::ToPlaneCollider(PlaneCollider* planeCollider)
{
	planeCollider->SetDistance(distance);
	planeCollider->SetRotation(worldTransform->rotation);
	planeCollider->SetBaseNormal(baseNormal);
}

void PolygonCollider::UpdateVertices()
{
	for (Vector3& vertex : vertices) { vertex *= worldTransform->matWorld; }
}

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

void WristerEngine::_2D::Base2DCollider::Initialize(Sprite* transform_, CollisionShapeType shapeType_, const std::string& colliderName_)
{
	transform = transform_;
	shapeType = shapeType_;
	colliderName = colliderName_;
}

std::map<std::string, Vector2> WristerEngine::_2D::BoxCollider::GetVertex() const
{
	std::map<std::string, Vector2> ans;
	ans["LT"] = ans["RB"] = ans["LB"] = ans["RT"] = transform->position;
	// ·•ª
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

void WristerEngine::_2D::ColliderGroup::AddCollider(Sprite* transform, CollisionShapeType shapeType, const std::string& colliderName, const Option* option)
{
	std::unique_ptr<Base2DCollider> newCollider;
	switch (shapeType)
	{
	case WristerEngine::_2D::CollisionShapeType::Box:
		newCollider = std::make_unique<_2D::BoxCollider>();
		break;
	case WristerEngine::_2D::CollisionShapeType::TwoRay:
		assert(option);
		newCollider = std::make_unique<TwoRayCollider>(option->fov);
		break;
	default:
		break;
	}

	newCollider->Initialize(transform, shapeType, colliderName);
	colliders.push_back(move(newCollider));
}

void WristerEngine::_2D::ColliderGroup::DeleteCollider(const std::string& colliderName)
{
	colliders.remove_if([&](const std::unique_ptr<Base2DCollider>& collider) { return collider->GetColliderName() == colliderName; });
}

void WristerEngine::_2D::ColliderGroup::AddCollisionPair(size_t myIndex, size_t youIndex)
{
	collisionPair[myIndex].push_back(youIndex);
}

void WristerEngine::_2D::ColliderGroup::DeletePair()
{
	for (auto& pair : collisionPair)
	{
		pair.second.clear();
	}
	collisionPair.clear();
}

const std::string WristerEngine::_2D::ColliderGroup::GetColliderName(size_t index) const
{
	if (colliders.size() <= index) { return "Null"; }
	auto itr = colliders.begin();
	for (size_t i = 0; i < index; i++)
	{
		itr++;
	}
	return itr->get()->GetColliderName();
}
