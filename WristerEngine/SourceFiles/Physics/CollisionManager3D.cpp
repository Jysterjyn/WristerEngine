#include "CollisionManager3D.h"
#include <algorithm>
#include "ImGuiManager.h"
#include <imgui.h>
using namespace std;
using namespace WE;

CollisionManager* CollisionManager::GetInstance()
{
	static CollisionManager instance;
	return &instance;
}

ColliderGroup* CollisionManager::AddGroup(const std::string& groupName)
{
	if (!colliderGroups.contains(groupName))
	{
		std::unique_ptr<ColliderGroup> newGroup = std::make_unique<ColliderGroup>();
		colliderGroups[groupName] = std::move(newGroup);
	}
	return colliderGroups[groupName].get();
}

void CollisionManager::CheckCollisions()
{
	for (auto& colliderGroup : colliderGroups) { colliderGroup.second->Update(); }

	auto itrA = colliderGroups.begin();
	for (; itrA != colliderGroups.end(); itrA++)
	{
		auto itrB = itrA;
		itrB++;
		for (; itrB != colliderGroups.end(); itrB++)
		{
			ColliderGroup* groupA = itrA->second.get();
			ColliderGroup* groupB = itrB->second.get();
			if (!CheckFiltering(groupA, groupB)) { continue; }
			if (!Check2Groups(groupA, groupB)) { continue; }
		}
	}

	for (auto& group : colliderGroups)
	{
		group.second->CallCollision();
	}
}

bool CollisionManager::CheckFiltering(const CollisionInfo* infoA, const CollisionInfo* infoB)
{
	return
		infoA->GetAttribute() & infoB->GetMask() &&
		infoB->GetAttribute() & infoA->GetMask();
}

bool CollisionManager::Check2Groups(ColliderGroup* groupA, ColliderGroup* groupB)
{
	for (const auto& colliderA : *groupA->GetColliders()) {
		for (const auto& colliderB : *groupB->GetColliders())
		{
			if (!CheckFiltering(colliderA.get(), colliderB.get())) { continue; }

			std::list<BaseCollider*> colliderPair({ colliderA.get(),colliderB.get() });
			colliderPair.sort([](BaseCollider* c1, BaseCollider* c2)
				{
					return c1->GetShapeType() < c2->GetShapeType();
				});

			if (colliderPair.back()->GetShapeType() == CollisionShapeType::Ray)
			{
				colliderPair.reverse();
			}

			if (Check2Collisions(colliderPair.front(), colliderPair.back()))
			{
				CollisionPair pairA(colliderA.get(), colliderB.get(), *this);
				CollisionPair pairB(colliderB.get(), colliderA.get(), *this);
				groupA->AddCollisionPair(pairA);
				groupB->AddCollisionPair(pairB);
				Reset();
			}
		}
	}

	return !groupA->GetCollisionPairs().empty();
}

bool CollisionManager::Check2Collisions(BaseCollider* colliderA, BaseCollider* colliderB)
{
	CollisionShapeType aST = colliderA->GetShapeType();
	CollisionShapeType bST = colliderB->GetShapeType();

	assert(aST != CollisionShapeType::Unknown);
	assert(bST != CollisionShapeType::Unknown);

	switch (aST)
	{
	case CollisionShapeType::Sphere:
	{
		SphereCollider* sphere = static_cast<SphereCollider*>(colliderA);

		switch (bST)
		{
		case CollisionShapeType::Sphere:

			return Check2Spheres(sphere, static_cast<SphereCollider*>(colliderB));

		case CollisionShapeType::Plane:

			return CheckSpherePlane(sphere, static_cast<PlaneCollider*>(colliderB));

		case CollisionShapeType::Triangle:

			return CheckSphereTriangle(sphere, static_cast<TriangleCollider*>(colliderB));

		case CollisionShapeType::Mesh:

			return CheckSphereMesh(sphere, static_cast<MeshCollider*>(colliderB));
		}
	}
	break;

	case CollisionShapeType::Box:
		if (aST == bST)
		{
			return
				Check2Boxes(static_cast<BoxCollider*>(colliderA), static_cast<BoxCollider*>(colliderB));
		}
		break;

	case CollisionShapeType::IncludeBox:
		if (aST == bST)
		{
			return
				Check2IncludeBoxes(static_cast<IncludeCollider*>(colliderA), static_cast<IncludeCollider*>(colliderB));
		}
		break;

	case CollisionShapeType::Ray:
	{
		RayCollider* ray = static_cast<RayCollider*>(colliderA);

		switch (bST)
		{
		case CollisionShapeType::Sphere:

			return CheckRaySphere(ray, static_cast<SphereCollider*>(colliderB));

		case CollisionShapeType::Plane:

			return CheckRayPlane(ray, static_cast<PlaneCollider*>(colliderB));

		case CollisionShapeType::Triangle:

			return CheckRayTriangle(ray, static_cast<TriangleCollider*>(colliderB));

		case CollisionShapeType::Mesh:

			return CheckRayMesh(ray, static_cast<MeshCollider*>(colliderB));
		}
	}
	}

	return false;
}

bool CollisionManager::Check2Spheres(const SphereCollider* sphereA, const SphereCollider* sphereB)
{
	// 値の取得
	Vector3 centerA = sphereA->GetCenterPosition();
	Vector3 centerB = sphereB->GetCenterPosition();
	float radA = sphereA->GetRadius();
	float radB = sphereB->GetRadius();

	//判定対象の座標
	Vector3 vecAB = centerA - centerB;
	float dist = Dot(vecAB, vecAB);
	//判定対象の半径
	float radAB = radA + radB;

	if (dist > radAB * radAB) { return false; }

	// Aの半径が0の時座標はBの中心　Bの半径が0の時座標はAの中心　となるよう補完
	float t = radB / radAB;
	inter = Lerp(centerA, centerB, t);
	// 押し出すベクトルを計算
	float rejectLen = radAB - sqrtf(dist);
	reject = Normalize(vecAB) * rejectLen;
	return true;
}

bool CollisionManager::CheckSpherePlane(const SphereCollider* sphere, const PlaneCollider* plane)
{
	// 座標系の原点から球の中心座標への距離
	float dist = Dot(sphere->GetCenterPosition(), plane->GetNormal());
	// 平面の原点座標を減算することで、平面と球の中心との距離が出る
	dist -= plane->GetDistance();
	// 距離の絶対値が半径より大きければ当たっていない
	if (fabsf(dist) > sphere->GetRadius()) { return false; }
	// 疑似交点を計算
	// 平面上の最近接点を疑似交点とする
	inter = -dist * plane->GetNormal() + sphere->GetCenterPosition();

	return true;
}

static Vector3 ClosestPtPoint2Triangle(const Vector3& point, const TriangleCollider* triangle)
{
	const std::array<Vector3, 3> p = triangle->GetVertices();
	// pointがp[0]の外側の頂点領域の中にあるかどうかチェック
	Vector3 p0_p1 = p[1] - p[0];
	Vector3 p0_p2 = p[2] - p[0];
	Vector3 p0_pt = point - p[0];

	float d1 = Dot(p0_p1, p0_pt);
	float d2 = Dot(p0_p2, p0_pt);

	if (d1 <= 0.0f && d2 <= 0.0f)
	{
		// p[0]が最近傍
		return p[0];
	}

	// pointがp[1]の外側の頂点領域の中にあるかどうかチェック
	Vector3 p1_pt = point - p[1];

	float d3 = Dot(p0_p1, p1_pt);
	float d4 = Dot(p0_p2, p1_pt);

	if (d3 >= 0.0f && d4 <= d3)
	{
		// p[1]が最近傍
		return p[1];
	}

	// pointがp0_p1の辺領域の中にあるかどうかチェックし、あればpointのp0_p1上に対する射影を返す
	float vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		float v = d1 / (d1 - d3);
		return p[0] + v * p0_p1;
	}

	// pointがp[2]の外側の頂点領域の中にあるかどうかチェック
	Vector3 p2_pt = point - p[2];

	float d5 = Dot(p0_p1, p2_pt);
	float d6 = Dot(p0_p2, p2_pt);
	if (d6 >= 0.0f && d5 <= d6)
	{
		return p[2];
	}

	// pointがp0_p2の辺領域の中にあるかどうかチェックし、あればpointのp0_p2上に対する射影を返す
	float vb = d5 * d2 - d1 * d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		return p[0] + w * p0_p2;
	}

	// pointがp1_p2の辺領域の中にあるかどうかチェックし、あればpointのp1_p2上に対する射影を返す
	float va = d3 * d6 - d5 * d4;
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		return p[1] + w * (p[2] - p[1]);
	}

	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	return p[0] + p0_p1 * v + p0_p2 * w;
}

bool CollisionManager::CheckSphereTriangle(const SphereCollider* sphere, const TriangleCollider* triangle)
{
	// 値の取得
	Vector3 spherePos = sphere->GetCenterPosition();
	float sphereRad = sphere->GetRadius();
	Vector3 triNormal = triangle->GetNormal();

	// 球の中心に対する最近接点である三角形上にある点pを見つける
	Vector3 p = ClosestPtPoint2Triangle(spherePos, triangle);
	// 点pと球の中心の差分ベクトル
	Vector3 v = p - spherePos;
	// 距離の二乗を求める
	float vLenSq = Dot(v, v);
	// 球と三角形の距離が半径以下なら当たっていない
	if (vLenSq > sphereRad * sphereRad) { return false; }
	// 疑似交点を計算
	// 三角形上の最近接点pを疑似交点とする
	inter = p;
	// 押し出すベクトルを計算
	float ds = Dot(spherePos, triNormal);
	float dt = Dot(triangle->GetVertices()[0], triNormal);
	float rejectLen = dt - ds + sphereRad;
	reject = triNormal * rejectLen;
	return true;
}

bool CollisionManager::Check2Boxes(const BoxCollider* boxA, const BoxCollider* boxB)
{
	Vector3 vecAB = boxA->GetCenterPosition() - boxB->GetCenterPosition();
	Vector3 radAB = boxA->GetRadius() + boxB->GetRadius();
	vecAB = vecAB.abs();

	return vecAB <= radAB;
}

bool CollisionManager::Check2IncludeBoxes(const IncludeCollider* iBoxA, const IncludeCollider* iBoxB)
{
	Vector3 vecAB = iBoxA->GetCenterPosition() - iBoxB->GetCenterPosition();
	// 使用フラグの取得
	array<bool, 3> isUse{};

	for (size_t i = 0; i < isUse.size(); i++)
	{
		isUse[i] = iBoxA->GetUseAxis()[i] && iBoxB->GetUseAxis()[i];
	}

	// 計算に使わない値を0にする
	if (!isUse[(size_t)IncludeCollider::Axis::X]) { vecAB.x = 0; }
	if (!isUse[(size_t)IncludeCollider::Axis::Y]) { vecAB.y = 0; }
	if (!isUse[(size_t)IncludeCollider::Axis::Z]) { vecAB.z = 0; }

	return vecAB.Length() <= IncludeCollider::GetIncludeRadius();
}

bool CollisionManager::CheckRayPlane(const RayCollider* ray, const PlaneCollider* plane)
{
	const float epsilon = 1.0e-5f; // 誤差吸収用の微小な値
	// 面法線とレイの方向ベクトルの内積
	float d1 = Dot(plane->GetNormal(), ray->GetDir());
	// 裏面には当たらない
	if (d1 > -epsilon) { return false; }
	// 始点と原点の距離(平面の法線方向)
	// 面法線とレイの始点座標(位置ベクトル)の内積
	float d2 = Dot(plane->GetNormal(), ray->GetStartPos());
	// 始点と平面の距離(レイ方向)
	float t = (plane->GetDistance() - d2) / d1;
	// 交点が始点より後ろにあるので当たらない
	if (t < 0) { return false; }
	// 距離を書き込む
	distance = t;
	// 交点を計算
	inter = ray->GetStartPos() + t * ray->GetDir();
	return true;
}

bool CollisionManager::CheckRayTriangle(const RayCollider* ray, const TriangleCollider* triangle)
{
	// 三角形が乗っている平面を算出
	PlaneCollider plane(*triangle);

	// レイと平面が当たっていなければ、当たっていない
	if (!CheckRayPlane(ray, &plane)) { return false; }
	// レイと平面が当たっていたので、距離と座標が書き込まれた
	// レイと平面の交点が三角形の内側にあるか判定
	const float epsilon = 1.0e-5f; // 誤差吸収用の微小な値

	size_t vertexSize = triangle->GetVertices().size();
	for (size_t i = 0; i < vertexSize; i++)
	{
		// 辺pi_p(i+1)について
		Vector3 pt_px = triangle->GetVertices()[i] - *inter;
		Vector3 px_py = triangle->GetVertices()[(i + 1) % vertexSize] - triangle->GetVertices()[i];
		Vector3 m = Cross(pt_px, px_py);
		// 辺の外側であれば当たっていないので判定を打ち切る
		if (Dot(m, triangle->GetNormal()) < -epsilon)
		{
			Reset();
			return false;
		}
	}

	// 内側なので当たっている
	return true;
}

bool CollisionManager::CheckRaySphere(const RayCollider* ray, const SphereCollider* sphere)
{
	Vector3 m = ray->GetStartPos() - sphere->GetCenterPosition();
	float b = Dot(m, ray->GetDir());
	float c = Dot(m, m) - sphere->GetRadius() * sphere->GetRadius();
	// レイの始点がsphereの外側にあり(c > 0)、レイが離れていく方向を差している場合(b > 0)、当たらない
	if (c > 0.0f && b > 0.0f) { return false; }

	float discr = b * b - c; // 判別式
	// 負の判別式はレイが球から外れている
	if (discr < 0.0f) { return false; }

	// レイは球と交差している
	float t = -b - sqrtf(discr); // 交差する最小の値tを計算
	// tが負である場合、レイは球の内側から開始しているのでtを0にクランプ
	t = max(t, 0.0f);

	distance = t;
	inter = ray->GetStartPos() + t * ray->GetDir();
	return true;
}

bool CollisionManager::CheckSphereMesh(const SphereCollider* sphere, const MeshCollider* mesh)
{
	// オブジェクトのローカル座標系での球を得る（半径はXスケールを参照)
	SphereCollider localSphere(true);
	localSphere.SetCenterPosition(sphere->GetCenterPosition() * mesh->GetInvMatWorld());
	localSphere.SetRadius(sphere->GetRadius() * mesh->GetInvMatWorld().GetVector(0).Length());

	for (const auto& tri : mesh->GetTriangles())
	{
		if (!CheckSphereTriangle(&localSphere, tri.get())) { continue; }
		Matrix4 matWorld = mesh->GetMatWorld();
		inter = *inter * matWorld;
		matWorld.SetVector({}, 3);
		reject = *reject * matWorld;
		return true;
	}

	return false;
}

bool CollisionManager::CheckRayMesh(const RayCollider* ray, const MeshCollider* mesh)
{
	// オブジェクトのローカル座標系でのレイを得る
	RayCollider localRay(true);
	localRay.SetStartPos(ray->GetStartPos() * mesh->GetInvMatWorld());
	Matrix4 invTransformMat = mesh->GetInvMatWorld();
	invTransformMat.SetVector({}, 3);
	localRay.SetDir(ray->GetDir() * invTransformMat);

	for (const auto& tri : mesh->GetTriangles())
	{
		if (!CheckRayTriangle(&localRay, tri.get())) { continue; }
		inter = *inter * mesh->GetMatWorld();
		Vector3 sub = *inter - ray->GetStartPos();
		distance = Dot(sub, ray->GetDir());
		return true;
	}

	return false;
}

bool CollisionManager::Raycast(const RayCollider* ray, uint32_t attribute, RaycastHit* hitInfo, const float maxDistance)
{
	bool result = false;
	//今までで最も近いコライダーを記録するためのイテレータ
	BaseCollider* it_hit = nullptr;
	//今までで最も近いコライダーの距離を記録する変数
	float tempDistance = maxDistance;
	Vector3 tempInter;

	//全コライダーと総当たりチェック
	for (auto& group : colliderGroups)
	{
		if (!(group.second->GetAttribute() & attribute))continue;

		for (auto& it : *group.second->GetColliders())
		{
			BaseCollider* colA = it.get();
			//属性が合わない場合スキップ
			if (!(colA->GetAttribute() & attribute))continue;

			//球の場合
			if (colA->GetShapeType() == CollisionShapeType::Sphere)
			{
				SphereCollider* sphere = dynamic_cast<SphereCollider*>(colA);
				//当たらなければ除外
				if (!CheckRaySphere(ray, sphere))continue;
				//距離が最小でなければ除外
				if (distance >= tempDistance)continue;
				//今までで最も近いので記録を取る
				result = true;
				tempDistance = *distance;
				tempInter = *inter;
				it_hit = it.get();
			}
			//メッシュの場合
			else if (colA->GetShapeType() == CollisionShapeType::Mesh)
			{
				MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);
				//当たらなければ除外
				if (!CheckRayMesh(ray, meshCollider))continue;
				//距離が最小でなければ除外
				if (distance >= tempDistance)continue;
				//今までで最も近いので記録を取る
				result = true;
				tempDistance = *distance;
				tempInter = *inter;
				it_hit = it.get();
			}
		}
	}
	//最終的に何かに当たっていれば結果を書き込む
	if (result && hitInfo)
	{
		hitInfo->distance = tempDistance;
		hitInfo->inter = tempInter;
		hitInfo->collider = it_hit;
	}

	Reset();

	return result;
}

void WristerEngine::CollisionManager::QuerySphere(const SphereCollider* sphere, QueryCallback* callback, uint32_t attribute)
{
	assert(callback);

	// 全てのコライダーと総当りチェック
	for (auto& group : colliderGroups) 
	{
		// 属性が合わなければスキップ
		if (!(group.second->GetAttribute() & attribute)) { continue; }

		for (auto& it : *group.second->GetColliders())
		{
			BaseCollider* col = it.get();
			// 属性が合わなければスキップ
			if (!(col->GetAttribute() & attribute)) { continue; }

			// 球
			if (col->GetShapeType() == CollisionShapeType::Sphere) {
				SphereCollider* sphereB = dynamic_cast<SphereCollider*>(col);

				if (!Check2Spheres(sphere, sphereB)) continue;

				// 交差情報をセット
				QueryHit info;
				info.collider = col;
				info.inter = *inter;
				info.reject = *reject;

				// クエリーコールバック呼び出し
				if (!callback->OnQueryHit(info)) {
					// 戻り値がfalseの場合、継続せず終了
					return;
				}
			}
			// メッシュ
			else if (col->GetShapeType() == CollisionShapeType::Mesh) {
				MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(col);

				if (!CheckSphereMesh(sphere, meshCollider)) continue;

				// 交差情報をセット
				QueryHit info;
				info.collider = col;
				info.inter = *inter;
				info.reject = *reject;

				// クエリーコールバック呼び出し
				if (!callback->OnQueryHit(info)) {
					// 戻り値がfalseの場合、継続せず終了
					return;
				}
			}
		}
	}
	Reset();
}

//bool CollisionManager::Check2DCollision2Boxes(const std::array<_2D::Base2DCollider*, 2>& colliders)
//{
//	std::array<const _2D::BoxCollider*, 2> box2DColliders{};
//	for (size_t i = 0; i < colliders.size(); i++)
//	{
//		box2DColliders[i] = dynamic_cast<_2D::BoxCollider*>(colliders[i]);
//	}
//
//	std::array<Vector2, 2> posCenter;
//	std::array<const _2D::Sprite*, 2> trans{};
//
//	for (size_t i = 0; i < box2DColliders.size(); i++)
//	{
//		trans[i] = box2DColliders[i]->GetTransform();
//		// 中心点を計算
//		auto pos = box2DColliders[i]->GetVertex();
//		posCenter[i] = Half<Vector2>(pos["LT"] + pos["RB"]);
//	}
//
//	// 当たり判定
//	if (std::abs(posCenter[0].x - posCenter[1].x) <= Half(trans[0]->size.x + trans[1]->size.x) &&
//		std::abs(posCenter[0].y - posCenter[1].y) <= Half(trans[0]->size.y + trans[1]->size.y))
//	{
//		return true;
//	}
//	return false;
//}
//
//bool CollisionManager::Check2DCollisionBox2Rays(const std::array<_2D::Base2DCollider*, 2>& colliders)
//{
//	const _2D::TwoRayCollider* rayCollider = nullptr;
//	const _2D::BoxCollider* boxCollider = nullptr;
//	for (auto collider : colliders)
//	{
//		if (collider->GetShapeType() == _2D::CollisionShapeType::Box)
//		{
//			boxCollider = dynamic_cast<_2D::BoxCollider*>(collider);
//		}
//		if (collider->GetShapeType() == _2D::CollisionShapeType::TwoRay)
//		{
//			rayCollider = dynamic_cast<_2D::TwoRayCollider*>(collider);
//		}
//	}
//
//	const _2D::Sprite* rayTrans = rayCollider->GetTransform();
//
//	// 視野角を計算
//	float leftRot = Angle(90) + rayCollider->GetFOV() + rayTrans->rotation;
//	float rightRot = Angle(90) - rayCollider->GetFOV() + rayTrans->rotation;
//
//	// ボックスの左上端と右下端の座標を求める
//	const _2D::Sprite* pSprite = boxCollider->GetTransform();
//	Vector2 pPosLT, pPosRB;
//	pPosLT = pPosRB = pSprite->position;
//
//	pPosLT -= Vector2(pSprite->size.x * pSprite->anchorPoint.x, pSprite->size.y * pSprite->anchorPoint.y);
//	pPosRB += Vector2(pSprite->size.x * (1.0f - pSprite->anchorPoint.x), pSprite->size.y * (1.0f - pSprite->anchorPoint.y));
//
//	// 左上の接触判定
//	Vector2 vec = Normalize(Vector2(std::cos(rightRot), std::sin(rightRot)));
//	Vector2 toEyePlayerLT = Normalize(pPosLT - rayTrans->position);
//	float crossLT = Cross(vec, Normalize(toEyePlayerLT));
//
//	// 右下の接触判定
//	vec = Normalize(Vector2(std::cos(leftRot), std::sin(leftRot)));
//	Vector2 toEyePlayerRB = Normalize(pPosRB - rayTrans->position);
//	float crossRB = Cross(vec, Normalize(toEyePlayerRB));
//
//	if (crossRB < 0 && crossLT > 0) { return true; }
//
//	Vector2 pPosLB, pPosRT;
//	pPosLB = pPosRT = pSprite->position;
//
//	pPosLB += Vector2(-pSprite->size.x * pSprite->anchorPoint.x, pSprite->size.y * (1.0f - pSprite->anchorPoint.y));
//	pPosRT += Vector2(pSprite->size.x * (1.0f - pSprite->anchorPoint.x), -pSprite->size.y * pSprite->anchorPoint.y);
//
//	// 左下の接触判定
//	vec = Normalize(Vector2(std::cos(rightRot), std::sin(rightRot)));
//	Vector2 toEyePlayerLB = Normalize(pPosLB - rayTrans->position);
//	float crossLB = Cross(vec, Normalize(toEyePlayerLB));
//
//	// 右上の接触判定
//	vec = Normalize(Vector2(std::cos(leftRot), std::sin(leftRot)));
//	Vector2 toEyePlayerRT = Normalize(pPosRT - rayTrans->position);
//	float crossRT = Cross(vec, Normalize(toEyePlayerRT));
//
//	return crossRT < 0 && crossLB > 0;
//}
//
//bool CollisionManager::CheckCollisionRayBox(RayCollider* colliderA, BoxCollider* colliderB)
//{
//	if (!CheckFiltering(colliderA, colliderB)) { return false; }
//	PolygonCollider pCollider;
//	Vector3 boxPos = colliderB->GetCenterPosition();
//	Vector3 boxRad = colliderB->GetRadius3D();
//	// 上底左奥
//	pCollider.AddVertices({ boxPos.x - boxRad.x,boxPos.y + boxRad.y,boxPos.z + boxRad.z });
//	// 上底右奥
//	pCollider.AddVertices({ boxPos.x + boxRad.x,boxPos.y + boxRad.y,boxPos.z + boxRad.z });
//	// 上底右前
//	pCollider.AddVertices({ boxPos.x + boxRad.x,boxPos.y + boxRad.y,boxPos.z - boxRad.z });
//	// 上底左前
//	pCollider.AddVertices({ boxPos.x - boxRad.x,boxPos.y + boxRad.y,boxPos.z - boxRad.z });
//
//	pCollider.SetBaseNormal({ Vector3::MakeAxis(Axis::Y) });
//
//	return CheckCollisionRayPolygon(colliderA, &pCollider);
//}
//
//bool CollisionManager::CheckCollision2ColliderGroups(_2D::Collider* groupA, _2D::Collider* groupB)
//{
//	if (!CheckFiltering(groupA, groupB)) { return false; }
//
//	const auto& collidersA = groupA->GetColliders();
//	const auto& collidersB = groupB->GetColliders();
//
//	bool isHitGroup = false;
//	size_t aIndex = 0;
//
//	// 2つのコライダーグループの全てのコライダーと当たり判定を取る
//	std::map<_2D::CollisionShapeType, size_t> shapeTypeCount;
//	auto itrA = collidersA.begin();
//	for (; itrA != collidersA.end(); itrA++, aIndex++)
//	{
//		bool isHitPair = false;
//		auto itrB = collidersB.begin();
//		size_t bIndex = 0;
//		for (; itrB != collidersB.end(); itrB++, bIndex++)
//		{
//			shapeTypeCount.clear();
//			shapeTypeCount[itrA->get()->GetShapeType()]++;
//			shapeTypeCount[itrB->get()->GetShapeType()]++;
//			// ボックスとボックスの当たり判定
//			if (shapeTypeCount[_2D::CollisionShapeType::Box] == 2)
//			{
//				isHitPair = Check2DCollision2Boxes({ itrA->get(),itrB->get() });
//			}
//			// ボックスと2レイの当たり判定
//			else if (shapeTypeCount[_2D::CollisionShapeType::Box] == shapeTypeCount[_2D::CollisionShapeType::TwoRay])
//			{
//				isHitPair = Check2DCollisionBox2Rays({ itrA->get(),itrB->get() });
//			}
//
//			if (isHitPair)
//			{
//				// コリジョンペアの登録
//				groupA->AddCollisionPair(aIndex, bIndex);
//				groupB->AddCollisionPair(bIndex, aIndex);
//				isHitGroup = true;
//			}
//		}
//	}
//
//	return isHitGroup;
//}
//
//void CollisionManager::Check2DCollisions()
//{
//	for (auto& collider : _2DColliders)
//	{
//		collider->DeletePair();
//	}
//
//	auto itrA = _2DColliders.begin();
//	for (; itrA != _2DColliders.end(); itrA++)
//	{
//		list<_2D::Collider*>::iterator itrB = itrA;
//		itrB++;
//		for (; itrB != _2DColliders.end(); itrB++)
//		{
//			if (!CheckCollision2ColliderGroups(*itrA, *itrB)) { continue; }
//
//			(*itrA)->OnCollision(*itrB);
//			(*itrB)->OnCollision(*itrA);
//		}
//	}
//}