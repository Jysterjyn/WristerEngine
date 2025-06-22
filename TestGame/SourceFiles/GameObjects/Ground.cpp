#include "Ground.h"
#include <CollisionInfo.h>
#include <Random.h>

void Ground::Initialize(const Vector3& pos, float scale)
{
	WE::Collider::Initialize("Ground", WE::CollisionInfo(ChangeVal(CollisionAttribute::Landshape)));

	std::array<std::string, 10> nametable;
	for (size_t i = 0; i < nametable.size() - 2; i++)
	{
		nametable[i] = "ground";
	}
	nametable[8] = "cube";
	nametable[9] = "pyramid";

	WE::Random_Int rand(0, 9);
	object = mm->Create(nametable[rand()], true);
	object->transform.scale *= scale;
	object->transform.translation = pos;
	collider = AddCollider<WE::MeshCollider>();
	collider->SetTransform(&object->transform);
	collider->ConstructTriangles(object->GetMesh());
	object->transform.Update();
	collider->Update();
}
