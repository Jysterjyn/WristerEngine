#include "Player.h"
#include "Input.h"
#include "ParticleManager.h"
#include "CollisionInfo.h"
#include <PrimitiveDrawer.h>

void Player::Initialize()
{
	object = WE::_3D::ModelManager::GetInstance()->Create("chr_sword");

	// コライダーの追加
	float radius = 0.6f;
	Collider::Initialize("Player");
	group->SetAttribute(ChangeVal(CollisionAttribute::Allies));
	sphere = static_cast<WE::SphereCollider*>(AddCollider(WE::CollisionShapeType::Sphere));
	sphere->SetAttribute(ChangeVal(CollisionAttribute::Allies));
	sphere->SetOffset({ 0,radius,0 });
	sphere->SetRadius(radius);
	sphere->SetTransform(&object->transform);
	ray = static_cast<WE::RayCollider*>(AddCollider(WE::CollisionShapeType::Ray));
	ray->SetAttribute(ChangeVal(CollisionAttribute::Allies));
}

void Player::Update()
{
	WE::Input* input = WE::Input::GetInstance();

	WE::_3D::Transform& transform = object->transform;
	// A,Dで旋回
	if (input->IsInput(WE::Key::A)) {
		transform.rotation.y -= Angle(2);
	}
	else if (input->IsInput(WE::Key::D)) {
		transform.rotation.y += Angle(2);
	}

	// 移動ベクトルをY軸周りの角度で回転
	Vector3 move = { 0,0,0.1f };
	Matrix4 matRot = Matrix4::RotateY(transform.rotation.y);
	move *= matRot;

	// 向いている方向に移動
	if (input->IsInput(WE::Key::S)) {
		transform.translation -= move;
	}
	else if (input->IsInput(WE::Key::W)) {
		transform.translation += move;
	}

	// 落下処理
	if (!onGround) {
		// 下向き加速度
		const float fallAcc = -0.01f;
		const float fallVYMin = -0.5f;
		// 加速
		fallV.y = max(fallV.y + fallAcc, fallVYMin);
		// 移動
		transform.translation += fallV;
	}
	// ジャンプ操作
	else if (input->IsTrigger(WE::Key::Space)) {
		onGround = false;
		const float jumpVYFist = 0.2f;
		fallV = { 0, jumpVYFist, 0 };
	}

	// 球の上端から球の下端までのレイキャスト
	Vector3 rayPos = sphere->GetCenterPosition();
	rayPos.y += sphere->GetRadius();
	ray->SetStartPos(rayPos);
	ray->SetDir({ 0,-1,0 });

	isCollision = false;

	//WE::_3D::PrimitiveDrawer* pd = WE::_3D::PrimitiveDrawer::GetInstance();
	//pd->ClearLines();
}

void Player::OnCollision()
{
	for (const auto& pair : GetCollisionPairs())
	{
		if (pair.my->GetShapeType() != WE::CollisionShapeType::Ray) { continue; }
		if (pair.other->GetAttribute() != ChangeVal(CollisionAttribute::Landshape)) { continue; }
		if (!pair.distance) { continue; }
		isCollision = true;
		// 接地状態
		if (onGround) {
			// スムーズに坂を下る為の吸着距離
			const float adsDistance = 0.2f;
			// 接地を維持
			if (pair.distance > sphere->GetRadius() * 2.0f + adsDistance) { continue; }
			onGround = true;
			object->transform.translation.y -= pair.distance.value() - sphere->GetRadius() * 2.0f;
		}
		// 落下状態
		else if (fallV.y <= 0.0f) {
			// 着地
			if (pair.distance > sphere->GetRadius() * 2.0f) { continue; }
			onGround = true;
			object->transform.translation.y -= pair.distance.value() - sphere->GetRadius() * 2.0f;
		}
	}
}
