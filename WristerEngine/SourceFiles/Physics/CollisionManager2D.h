#pragma once
#include "Collider2D.h"
#include <list>
#include <optional>

namespace WristerEngine::_2D
{
	// コライダー管理
	class CollisionManager2D final
	{
	private:
		// 個別当たり判定
		//bool Check2DCollision2Boxes(const std::array<_2D::Base2DCollider*, 2>& colliders);
		//bool Check2DCollisionBox2Rays(const std::array<_2D::Base2DCollider*, 2>& colliders);

	public:
		static CollisionManager2D* GetInstance();
		bool CheckFiltering(const CollisionInfo* infoA, const CollisionInfo* infoB);

		// 全当たり判定
		void CheckCollisions();
	};
}