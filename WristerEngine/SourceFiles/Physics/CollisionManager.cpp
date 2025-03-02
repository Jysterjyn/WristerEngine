#include "CollisionManager.h"
#include <algorithm>
#include "ImGuiManager.h"
#include <imgui.h>
using namespace std;
using namespace WristerEngine;

list<BoxCollider*> CollisionManager::boxColliders;
list<IncludeCollider*> CollisionManager::includeColliders;
list<SphereCollider*> CollisionManager::sphereColliders;
list<PlaneCollider*> CollisionManager::planeColliders;
list<PolygonCollider*> CollisionManager::polygonColliders;
list<RayCollider*> CollisionManager::rayColliders;
list<_2D::ColliderGroup*> CollisionManager::_2DColliders;

bool CollisionManager::CheckCollisionFiltering(BaseCollider* colliderA, BaseCollider* colliderB)
{
	return
		(UINT)colliderA->GetCollisionAttribute() & (UINT)colliderB->GetCollisionMask() &&
		(UINT)colliderB->GetCollisionAttribute() & (UINT)colliderA->GetCollisionMask();
}

bool CollisionManager::CheckCollisionFiltering(_2D::ColliderGroup* colliderA, _2D::ColliderGroup* colliderB)
{
	return
		(UINT)colliderA->GetCollisionAttribute() & (UINT)colliderB->GetCollisionMask() &&
		(UINT)colliderB->GetCollisionAttribute() & (UINT)colliderA->GetCollisionMask();
}

bool WristerEngine::CollisionManager::Check2DCollision2Boxes(const std::array<_2D::Base2DCollider*, 2>& colliders)
{
	std::array<const _2D::BoxCollider*, 2> box2DColliders{};
	for (size_t i = 0; i < colliders.size(); i++)
	{
		box2DColliders[i] = dynamic_cast<_2D::BoxCollider*>(colliders[i]);
	}

	std::array<Vector2, 2> posCenter;
	std::array<const _2D::Sprite*, 2> trans{};

	for (size_t i = 0; i < box2DColliders.size(); i++)
	{
		trans[i] = box2DColliders[i]->GetTransform();
		// 中心点を計算
		auto pos = box2DColliders[i]->GetVertex();
		posCenter[i] = Half<Vector2>(pos["LT"] + pos["RB"]);
	}

	// 当たり判定
	if (std::abs(posCenter[0].x - posCenter[1].x) <= Half(trans[0]->size.x + trans[1]->size.x) &&
		std::abs(posCenter[0].y - posCenter[1].y) <= Half(trans[0]->size.y + trans[1]->size.y))
	{
		return true;
	}
	return false;
}

bool WristerEngine::CollisionManager::Check2DCollisionBox2Rays(const std::array<_2D::Base2DCollider*, 2>& colliders)
{
	const _2D::TwoRayCollider* rayCollider = nullptr;
	const _2D::BoxCollider* boxCollider = nullptr;
	for (auto collider : colliders)
	{
		if (collider->GetShapeType() == _2D::CollisionShapeType::Box)
		{
			boxCollider = dynamic_cast<_2D::BoxCollider*>(collider);
		}
		if (collider->GetShapeType() == _2D::CollisionShapeType::TwoRay)
		{
			rayCollider = dynamic_cast<_2D::TwoRayCollider*>(collider);
		}
	}

	const _2D::Sprite* rayTrans = rayCollider->GetTransform();

	// 視野角を計算
	float leftRot = Angle(90) + rayCollider->GetFOV() + rayTrans->rotation;
	float rightRot = Angle(90) - rayCollider->GetFOV() + rayTrans->rotation;

	// ボックスの左上端と右下端の座標を求める
	const _2D::Sprite* pSprite = boxCollider->GetTransform();
	Vector2 pPosLT, pPosRB;
	pPosLT = pPosRB = pSprite->position;

	pPosLT -= Vector2(pSprite->size.x * pSprite->anchorPoint.x, pSprite->size.y * pSprite->anchorPoint.y);
	pPosRB += Vector2(pSprite->size.x * (1.0f - pSprite->anchorPoint.x), pSprite->size.y * (1.0f - pSprite->anchorPoint.y));

	// 左上の接触判定
	Vector2 vec = Normalize(Vector2(std::cos(rightRot), std::sin(rightRot)));
	Vector2 toEyePlayerLT = Normalize(pPosLT - rayTrans->position);
	float crossLT = Cross(vec, Normalize(toEyePlayerLT));

	// 右下の接触判定
	vec = Normalize(Vector2(std::cos(leftRot), std::sin(leftRot)));
	Vector2 toEyePlayerRB = Normalize(pPosRB - rayTrans->position);
	float crossRB = Cross(vec, Normalize(toEyePlayerRB));

	if (crossRB < 0 && crossLT > 0) { return true; }

	Vector2 pPosLB, pPosRT;
	pPosLB = pPosRT = pSprite->position;

	pPosLB += Vector2(-pSprite->size.x * pSprite->anchorPoint.x, pSprite->size.y * (1.0f - pSprite->anchorPoint.y));
	pPosRT += Vector2(pSprite->size.x * (1.0f - pSprite->anchorPoint.x), -pSprite->size.y * pSprite->anchorPoint.y);

	// 左下の接触判定
	vec = Normalize(Vector2(std::cos(rightRot), std::sin(rightRot)));
	Vector2 toEyePlayerLB = Normalize(pPosLB - rayTrans->position);
	float crossLB = Cross(vec, Normalize(toEyePlayerLB));

	// 右上の接触判定
	vec = Normalize(Vector2(std::cos(leftRot), std::sin(leftRot)));
	Vector2 toEyePlayerRT = Normalize(pPosRT - rayTrans->position);
	float crossRT = Cross(vec, Normalize(toEyePlayerRT));

	return crossRT < 0 && crossLB > 0;
}

bool CollisionManager::CheckCollision2Boxes(BoxCollider* colliderA, BoxCollider* colliderB)
{
	if (!CheckCollisionFiltering(colliderA, colliderB)) { return false; }

	Vector3 vecAB = colliderA->GetWorldPosition() - colliderB->GetWorldPosition();
	Vector3 radAB = colliderA->GetRadius3D() + colliderB->GetRadius3D();
	vecAB = vecAB.abs();

	return vecAB <= radAB;
}

bool CollisionManager::CheckCollision2IncludeObjects(IncludeCollider* colliderA, IncludeCollider* colliderB)
{
	if (!CheckCollisionFiltering(colliderA, colliderB)) { return false; }

	Vector3 vecAB = colliderA->GetWorldPosition() - colliderB->GetWorldPosition();
	// 使用フラグの取得
	array<bool, 3> isUse{};

	for (size_t i = 0; i < isUse.size(); i++)
	{
		isUse[i] = colliderA->GetUseAxis()[i] && colliderB->GetUseAxis()[i];
	}

	// 計算に使わない値を0にする
	if (!isUse[(size_t)IncludeCollider::Axis::X]) { vecAB.x = 0; }
	if (!isUse[(size_t)IncludeCollider::Axis::Y]) { vecAB.y = 0; }
	if (!isUse[(size_t)IncludeCollider::Axis::Z]) { vecAB.z = 0; }

	return vecAB.Length() <= IncludeCollider::GetIncludeRadius();
}

bool CollisionManager::CheckCollision2Spheres(SphereCollider* colliderA, SphereCollider* colliderB)
{
	if (!CheckCollisionFiltering(colliderA, colliderB)) { return false; }

	Vector3 vecAB = colliderA->GetWorldPosition() - colliderB->GetWorldPosition();
	float radAB = colliderA->GetRadius() + colliderB->GetRadius();

	return vecAB.Length() <= radAB;
}

bool CollisionManager::CheckCollisionSpherePlane(SphereCollider* colliderA, PlaneCollider* colliderB, Vector3* inter)
{
	// 座標系の原点から球の中心座標への距離
	float dist = Dot(colliderA->GetWorldPosition(), colliderB->GetNormal());
	// 平面の原点座標を減算することで、平面と球の中心との距離が出る
	dist -= colliderB->GetDistance();
	// 距離の絶対値が半径より大きければ当たっていない
	if (fabsf(dist) > colliderA->GetRadius()) { return false; }
	// 疑似交点を計算
	if (inter) { *inter = -dist * colliderB->GetNormal() + colliderA->GetWorldPosition(); } // 平面上の最近接点を疑似交点とする

	return true;
}

void ClosestPtPoint2Triangle(const Vector3& point, PolygonCollider* triangle, Vector3* closest)
{
	vector<Vector3> p0_p; p0_p.push_back({});
	p0_p.push_back(triangle->GetVertices()[1] - triangle->GetVertices()[0]);
	p0_p.push_back(triangle->GetVertices()[2] - triangle->GetVertices()[0]);
	vector<float> d; d.push_back(0);
	vector<float> v;

	for (size_t i = 0; i < 3; i++)
	{
		Vector3 pi_pt = point - triangle->GetVertices()[i];
		d.push_back(Dot(p0_p[1], pi_pt));
		d.push_back(Dot(p0_p[2], pi_pt));

		switch (i)
		{
		case 0:
			// pointがp0の外側の頂点領域の中にあるかどうかチェック
			if (d[1] <= 0.0f && d[2] <= 0.0f)
			{
				// p0が最近傍
				*closest = triangle->GetVertices()[0];
				return;
			}
			break;
		default:
			bool roopNum = i == 2;
			if (d[3 * i] >= 0.0f && d[4 + roopNum] <= d[3 * i])
			{
				// p[i]が最近傍
				*closest = triangle->GetVertices()[i];
				return;
			}

			// pointがp0_p[i]の辺領域の中にあるかどうかチェックし、あればpointのp0_p[i]上に対する射影を返す
			v.push_back(d[NumberLoop(1 + 4 * roopNum, 1, 6)] * d[NumberLoop(4 + 4 * roopNum, 1, 6)] -
				d[NumberLoop(3 + 4 * roopNum, 1, 6)] * d[NumberLoop(2 + 4 * roopNum, 1, 6)]);
			if (v[0 + roopNum] <= 0.0f && d[1 + roopNum] >= 0.0f && d[3 * i] <= 0.0f)
			{
				float v_ = d[1 + roopNum] / (d[1 + roopNum] - d[3 * i]);
				*closest = triangle->GetVertices()[0] + v_ * p0_p[i];
				return;
			}

			if (!roopNum) { continue; }
			// pointがp1_p2の辺領域の中にあるかどうかチェックし、あればpointのp1_p2上に対する射影を返す
			float va = d[3] * d[6] - d[5] * d[4];
			if (va <= 0.0f && (d[4] - d[3]) >= 0.0f && (d[5] - d[6]) >= 0.0f)
			{
				float w = (d[4] - d[3]) / ((d[4] - d[3]) + (d[5] - d[6]));
				*closest = triangle->GetVertices()[1] + w * (triangle->GetVertices()[2] - triangle->GetVertices()[1]);
				return;
			}

			float denom = 1.0f / (va + v[1] + v[0]);
			float v1 = v[1] * denom;
			float w = v[0] * denom;
			*closest = triangle->GetVertices()[0] + p0_p[1] * v1 + p0_p[2] * w;
			break;
		}
	}
}

bool CollisionManager::CheckCollisionSpherePolygon(SphereCollider* colliderA, PolygonCollider* colliderB, Vector3* inter)
{
	Vector3 p;
	// 球の中心に対する最近接点である三角形上にある点pを見つける
	ClosestPtPoint2Triangle(colliderA->GetWorldPosition(), colliderB, &p);
	// 点pと球の中心の差分ベクトル
	Vector3 v = p - colliderA->GetWorldPosition();
	float vLenSq = Dot(v, v);
	// 球と三角形の距離が半径以下なら当たっていない
	if (vLenSq > colliderA->GetRadius() * colliderA->GetRadius()) { return false; } // 三角形上の最近接点pを疑似交点とする
	// 疑似交点を計算
	if (inter) { *inter = p; }
	return true;
}

bool CollisionManager::CheckCollisionRayPlane(RayCollider* colliderA, PlaneCollider* colliderB, float* distance)
{
	if (!CheckCollisionFiltering(colliderA, colliderB)) { return false; }
	const float epsilon = 1.0e-5f; // 誤差吸収用の微小な値
	// 面法線とレイの方向ベクトルの内積
	float d1 = Dot(colliderB->GetNormal(), colliderA->GetRayDirection());
	// 裏面には当たらない
	if (d1 > -epsilon) { return false; }
	// 始点と原点の距離(平面の法線方向)
	// 面法線とレイの始点座標(位置ベクトル)の内積
	float d2 = Dot(colliderB->GetNormal(), colliderA->GetWorldPosition());
	// 始点と平面の距離(平面の法線方向)
	float dist = d2 - colliderB->GetDistance();
	// 始点と平面の距離(レイ方向)
	float t = dist / -d1;
	// 交点が始点より後ろにあるので当たらない
	if (t < 0) { return false; }
	// 距離を書き込む
	if (distance) { *distance = t; }
	// 交点を計算
	colliderB->SetInter(colliderA->GetWorldPosition() + t * colliderA->GetRayDirection());
	return true;
}

bool CollisionManager::CheckCollisionRayPolygon(RayCollider* colliderA, PolygonCollider* colliderB, float* distance)
{
	if (!CheckCollisionFiltering(colliderA, colliderB)) { return false; }
	// 三角形が乗っている平面を算出
	colliderB->ComputeDistance();
	// レイと平面が当たっていなければ当っていない
	PlaneCollider tempColliderB;
	colliderB->ToPlaneCollider(&tempColliderB);

	if (!CheckCollisionRayPlane(colliderA, &tempColliderB, distance)) { return false; }
	// レイと平面が当たっていたので、距離と座標が書き込まれた
	// レイと平面の交点が三角形の内側にあるか判定
	const float epsilon = 1.0e-5f; // 誤差吸収用の微小な値

	size_t vertexSize = colliderB->GetVertices().size();
	// 頂点数が2つ以下なら判定を打ち切る
	if (vertexSize <= 2) { return false; }

	for (size_t i = 0; i < vertexSize; i++)
	{
		// 辺pi_p(i+1)について
		Vector3 pt_px = colliderB->GetVertices()[i] - *tempColliderB.GetInter();
		Vector3 px_py = colliderB->GetVertices()[(i + 1) % vertexSize] - colliderB->GetVertices()[i];
		Vector3 m = Cross(pt_px, px_py);
		// 辺の外側であれば当たっていないので判定を打ち切る
		if (Dot(m, colliderB->GetNormal()) < -epsilon) { return false; }
	}

	// 内側なので当たっている
	return true;
}

bool CollisionManager::CheckCollisionRaySphere(RayCollider* colliderA, SphereCollider* colliderB, float* distance, Vector3* inter)
{
	Vector3 m = colliderA->GetWorldPosition() - colliderB->GetWorldPosition();
	float b = Dot(m, colliderA->GetRayDirection());
	float c = Dot(m, m) - colliderB->GetRadius() * colliderB->GetRadius();
	// レイの始点がsphereの外側にあり(c > 0)、レイが離れていく方向を差している場合(b > 0)、当たらない
	if (c > 0.0f && b > 0.0f) { return false; }

	float discr = b * b - c; // 判別式
	// 負の判別式はレイが球から外れている
	if (discr < 0.0f) { return false; }

	// レイは球と交差している
	float t = -b - sqrtf(discr); // 交差する最小の値tを計算
	// tが負である場合、レイは球の内側から開始しているのでtを0にクランプ
	if (t < 0) { t = 0.0f; }

	if (distance) { *distance = t; }
	if (inter) { *inter = colliderA->GetWorldPosition() + t * colliderA->GetRayDirection(); }
	return true;
}

bool CollisionManager::CheckCollisionRayBox(RayCollider* colliderA, BoxCollider* colliderB)
{
	if (!CheckCollisionFiltering(colliderA, colliderB)) { return false; }
	PolygonCollider pCollider;
	Vector3 boxPos = colliderB->GetWorldPosition();
	Vector3 boxRad = colliderB->GetRadius3D();
	// 上底左奥
	pCollider.AddVertices({ boxPos.x - boxRad.x,boxPos.y + boxRad.y,boxPos.z + boxRad.z });
	// 上底右奥
	pCollider.AddVertices({ boxPos.x + boxRad.x,boxPos.y + boxRad.y,boxPos.z + boxRad.z });
	// 上底右前
	pCollider.AddVertices({ boxPos.x + boxRad.x,boxPos.y + boxRad.y,boxPos.z - boxRad.z });
	// 上底左前
	pCollider.AddVertices({ boxPos.x - boxRad.x,boxPos.y + boxRad.y,boxPos.z - boxRad.z });

	pCollider.SetBaseNormal({ Vector3::MakeAxis(Axis::Y) });

	return CheckCollisionRayPolygon(colliderA, &pCollider);
}

bool WristerEngine::CollisionManager::CheckCollision2ColliderGroups(_2D::ColliderGroup* groupA, _2D::ColliderGroup* groupB)
{
	if (!CheckCollisionFiltering(groupA, groupB)) { return false; }

	const auto& collidersA = groupA->GetColliders();
	const auto& collidersB = groupB->GetColliders();

	bool isHitGroup = false;
	size_t aIndex = 0;

	// 2つのコライダーグループの全てのコライダーと当たり判定を取る
	std::map<_2D::CollisionShapeType, size_t> shapeTypeCount;
	auto itrA = collidersA.begin();
	for (; itrA != collidersA.end(); itrA++, aIndex++)
	{
		bool isHitPair = false;
		auto itrB = collidersB.begin();
		size_t bIndex = 0;
		for (; itrB != collidersB.end(); itrB++, bIndex++)
		{
			shapeTypeCount.clear();
			shapeTypeCount[itrA->get()->GetShapeType()]++;
			shapeTypeCount[itrB->get()->GetShapeType()]++;
			// ボックスとボックスの当たり判定
			if (shapeTypeCount[_2D::CollisionShapeType::Box] == 2)
			{
				isHitPair = Check2DCollision2Boxes({ itrA->get(),itrB->get() });
			}
			// ボックスと2レイの当たり判定
			else if (shapeTypeCount[_2D::CollisionShapeType::Box] == shapeTypeCount[_2D::CollisionShapeType::TwoRay])
			{
				isHitPair = Check2DCollisionBox2Rays({ itrA->get(),itrB->get() });
			}

			if (isHitPair)
			{
				// コリジョンペアの登録
				groupA->AddCollisionPair(aIndex, bIndex);
				groupB->AddCollisionPair(bIndex, aIndex);
				isHitGroup = true;
			}
		}
	}

	return isHitGroup;
}

void CollisionManager::CheckBoxCollisions()
{
	auto itrA = boxColliders.begin();
	for (; itrA != boxColliders.end(); itrA++)
	{
		list<BoxCollider*>::iterator itrB = itrA;
		itrB++;
		for (; itrB != boxColliders.end(); itrB++)
		{
			if (!CheckCollision2Boxes(*itrA, *itrB)) { continue; }

			(*itrA)->OnCollision(*itrB);
			(*itrB)->OnCollision(*itrA);
		}
	}
}

void CollisionManager::CheckIncludeCollisions()
{
	auto itrA = includeColliders.begin();
	for (; itrA != includeColliders.end(); itrA++)
	{
		list<IncludeCollider*>::iterator itrB = itrA;
		itrB++;
		for (; itrB != includeColliders.end(); itrB++)
		{
			if (!CheckCollision2IncludeObjects(*itrA, *itrB)) { continue; }

			(*itrA)->OnCollision(*itrB);
			(*itrB)->OnCollision(*itrA);
		}
	}
}

void CollisionManager::CheckSphereCollisions()
{
	auto itrA = sphereColliders.begin();
	for (; itrA != sphereColliders.end(); itrA++)
	{
		list<SphereCollider*>::iterator itrB = itrA;
		itrB++;
		for (; itrB != sphereColliders.end(); itrB++)
		{
			if (!CheckCollision2Spheres(*itrA, *itrB)) { continue; }

			(*itrA)->OnCollision(*itrB);
			(*itrB)->OnCollision(*itrA);
		}
	}
}

void CollisionManager::CheckSpherePlaneCollisions()
{
	for (SphereCollider* sphereCollider : sphereColliders) {
		for (PlaneCollider* planeCollider : planeColliders)
		{
			if (!CheckCollisionSpherePlane(sphereCollider, planeCollider)) { continue; }

			sphereCollider->OnCollision(planeCollider);
			planeCollider->OnCollision(sphereCollider);
		}
	}
}

void CollisionManager::CheckSpherePolygonCollisions()
{
	for (SphereCollider* sphereCollider : sphereColliders) {
		for (PolygonCollider* polygonCollider : polygonColliders)
		{
			if (!CheckCollisionSpherePolygon(sphereCollider, polygonCollider)) { continue; }

			sphereCollider->OnCollision(polygonCollider);
			polygonCollider->OnCollision(sphereCollider);
		}
	}
}

void CollisionManager::CheckRayPlaneCollisions()
{
	if (rayColliders.empty() || planeColliders.empty()) { return; }
	for (RayCollider* rayCollider : rayColliders) {
		for (PlaneCollider* planeCollider : planeColliders)
		{
			if (!CheckCollisionRayPlane(rayCollider, planeCollider)) { continue; }

			rayCollider->OnCollision(planeCollider);
			planeCollider->OnCollision(rayCollider);
		}
	}
}

void CollisionManager::CheckRayPolygonCollisions()
{
	if (rayColliders.empty() || polygonColliders.empty()) { return; }
	for (RayCollider* rayCollider : rayColliders) {
		for (PolygonCollider* polygonCollider : polygonColliders)
		{
			if (!CheckCollisionRayPolygon(rayCollider, polygonCollider)) { continue; }

			rayCollider->OnCollision(polygonCollider);
			polygonCollider->OnCollision(rayCollider);
		}
	}
}

void CollisionManager::CheckRaySphereCollisions()
{
	if (rayColliders.empty() || sphereColliders.empty()) { return; }
	for (RayCollider* rayCollider : rayColliders) {
		for (SphereCollider* sphereCollider : sphereColliders)
		{
			if (!CheckCollisionRaySphere(rayCollider, sphereCollider)) { continue; }

			rayCollider->OnCollision(sphereCollider);
			sphereCollider->OnCollision(rayCollider);
		}
	}
}

void CollisionManager::CheckRayBoxCollisions()
{
	for (RayCollider* rayCollider : rayColliders) {
		for (BoxCollider* boxCollider : boxColliders)
		{
			if (!CheckCollisionRayBox(rayCollider, boxCollider)) { continue; }

			rayCollider->OnCollision(boxCollider);
			boxCollider->OnCollision(rayCollider);
		}
	}
}

void CollisionManager::CheckRayCastCollision(RayCollider* collider)
{
	if (rayColliders.empty()) { return; }

	struct RayCastHit
	{
		float distance = 0;
		PlaneCollider* planeCollider = nullptr;
		PolygonCollider* polygonCollider = nullptr;
		SphereCollider* sphereCollider = nullptr;

		void OnCollision(RayCollider* collider)
		{
			if (this->planeCollider)
			{
				collider->OnCollision(this->planeCollider);
				this->planeCollider->OnCollision(collider);
			}
			else if (this->sphereCollider)
			{
				collider->OnCollision(this->sphereCollider);
				this->sphereCollider->OnCollision(collider);
			}
			else if (this->polygonCollider)
			{
				collider->OnCollision(this->polygonCollider);
				this->sphereCollider->OnCollision(collider);
			}
		}
	};

	float distance = 0;
	vector<RayCastHit> collisionInfo;

	for (PlaneCollider* planeCollider : planeColliders)
	{
		if (!CheckCollisionRayPlane(collider, planeCollider, &distance)) { continue; }
		RayCastHit newInfo;
		newInfo.distance = distance;
		newInfo.planeCollider = planeCollider;
		collisionInfo.push_back(newInfo);
	}
	for (PolygonCollider* polygonCollider : polygonColliders)
	{
		if (!CheckCollisionRayPolygon(collider, polygonCollider, &distance)) { continue; }
		RayCastHit newInfo;
		newInfo.distance = distance;
		newInfo.polygonCollider = polygonCollider;
		collisionInfo.push_back(newInfo);
	}
	for (SphereCollider* sphereCollider : sphereColliders)
	{
		if (!CheckCollisionRaySphere(collider, sphereCollider, &distance)) { continue; }
		RayCastHit newInfo;
		newInfo.distance = distance;
		newInfo.sphereCollider = sphereCollider;
		collisionInfo.push_back(newInfo);
	}

	// 何とも当っていない
	if (collisionInfo.empty()) { return; }
	if (collisionInfo.size() == 1)
	{
		collisionInfo[0].OnCollision(collider);
		return;
	}

	RayCastHit nearly = collisionInfo[0];
	for (size_t i = 1; i < collisionInfo.size(); i++)
	{
		if (nearly.distance > collisionInfo[i].distance) { nearly = collisionInfo[i]; }
	}
	nearly.OnCollision(collider);
}

void WristerEngine::CollisionManager::Check2DCollisions()
{
	for (auto& collider : _2DColliders)
	{
		collider->DeletePair();
	}

	auto itrA = _2DColliders.begin();
	for (; itrA != _2DColliders.end(); itrA++)
	{
		list<_2D::ColliderGroup*>::iterator itrB = itrA;
		itrB++;
		for (; itrB != _2DColliders.end(); itrB++)
		{
			if (!CheckCollision2ColliderGroups(*itrA, *itrB)) { continue; }

			(*itrA)->OnCollision(*itrB);
			(*itrB)->OnCollision(*itrA);
		}
	}
}

void CollisionManager::CheckAllCollisions()
{
	CheckBoxCollisions();
	CheckIncludeCollisions();
	CheckSphereCollisions();
	CheckSpherePlaneCollisions();
	CheckSpherePolygonCollisions();
	CheckRayPlaneCollisions();
	CheckRayPolygonCollisions();
	CheckRaySphereCollisions();
	CheckRayBoxCollisions();
	Check2DCollisions();
}