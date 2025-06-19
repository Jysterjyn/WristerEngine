#pragma once
#include "Object3d.h"
#include <Collider.h>

class Player : public WE::Collider
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	void OnCollision() override;

	void OnCollisionExit() override
	{
		if (!onGround) { return; }
		// 地面がないので落下
		onGround = false;
		fallV = {};
	}

private:
	WE::SphereCollider* sphere = nullptr;
	WE::RayCollider* ray = nullptr;
	bool isCollision = false;

	bool onGround = true;
	// 落下ベクトル
	Vector3 fallV;
	WE::_3D::Object3d* object = nullptr;
};

