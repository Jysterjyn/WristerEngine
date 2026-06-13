#pragma once
#include "Sprite.h"
#include <map>
#include <optional>
#include "CollisionInfo.h"

namespace WristerEngine
{
	namespace _2D
	{
		enum class CollisionShapeType
		{
			Unknown,
			Box,
			TwoRay,
			Sphere
		};

		struct Option
		{
			Angle fov;
		};

		//class Collider;

		class Base2DCollider
		{
		protected:
			Sprite* transform = nullptr;
			CollisionShapeType shapeType = CollisionShapeType::Unknown;
			std::string colliderName;

		public:
			virtual ~Base2DCollider() = default;

			// 初期化
			void Initialize(Sprite* transform, CollisionShapeType shapeType, const std::string& colliderName);

			// getter
			const Sprite* GetTransform() const { return transform; }
			CollisionShapeType GetShapeType() const { return shapeType; }
			const std::string GetColliderName() const { return colliderName; }
		};

		// 四角形コライダー
		class BoxCollider : public Base2DCollider
		{
		public:
			// 左上端と右下端の座標を求める
			std::map<std::string, Vector2> GetVertex() const;
		};

		// 1点から2方向に延びる線分との当たり判定（まだ不完全）
		class TwoRayCollider : public Base2DCollider
		{
			Angle fov; // 視野角

		public:
			TwoRayCollider(Angle fov_) { fov = fov_; }
			Angle GetFOV() const { return fov; }
		};

		class TestCircleCollider;

		// 仮コライダー判定クラス
		class TestCheckAllCircleCollision
		{
			std::list<TestCircleCollider*> colliders;

			bool Check2Circles(TestCircleCollider* a, TestCircleCollider* b);

			TestCheckAllCircleCollision() = default;
			~TestCheckAllCircleCollision() = default;
			TestCheckAllCircleCollision(const TestCheckAllCircleCollision&) = delete;
			TestCheckAllCircleCollision& operator=(const TestCheckAllCircleCollision&) = delete;

		public:
			static TestCheckAllCircleCollision* GetInstance()
			{
				static TestCheckAllCircleCollision instance;
				return &instance;
			}

			void CheckCircleCollisions();
			void Add(TestCircleCollider* a) { colliders.push_back(a); }
			void Delete(TestCircleCollider* a) 
			{
				colliders.remove(a); 
			}
			void Clear() { /*colliders.clear()*/; }
		};

		// 仮円コライダー
		class TestCircleCollider : public Base2DCollider, public CollisionInfo
		{
			TestCheckAllCircleCollision* collision = TestCheckAllCircleCollision::GetInstance();
			float radius = 0;

		public:
			virtual void OnCollision([[maybe_unused]] TestCircleCollider* other) {}
			float GetRadius() const { return radius; }
			void SetRadius(float radius_) { radius = radius_; }

			TestCircleCollider() { collision->Add(this); }
			~TestCircleCollider() 
			{
				//collision->Delete(this); 
			}
		};

		//class Collider
		//{
		//protected:
		//	std::list<std::unique_ptr<Base2DCollider>> colliders;
		//	CollisionAttribute collisionAttribute = CollisionAttribute::All;
		//	CollisionMask collisionMask = CollisionMask::All;
		//	std::map<size_t, std::vector<size_t>> collisionPair;

		//public:
		//	// コンストラクタ
		//	Collider();
		//	// 仮想デストラクタ
		//	virtual ~Collider();

		//	// コライダーの追加
		//	void AddCollider(Sprite* transform, CollisionShapeType shapeType, const std::string& colliderName, const Option* option = nullptr);
		//	// コライダーの削除
		//	void DeleteCollider(const std::string& colliderName);
		//	// コリジョンペアの追加
		//	void AddCollisionPair(size_t myIndex, size_t youIndex);
		//	// コリジョンペアの追加
		//	void DeletePair();

		//	// getter
		//	CollisionAttribute GetAttribute() const { return collisionAttribute; }
		//	CollisionMask GetMask() const { return collisionMask; }
		//	const std::string GetColliderName(size_t index) const;
		//	const std::list<std::unique_ptr<Base2DCollider>>& GetColliders() const { return colliders; }

		//	// 衝突コールバック関数
		//	virtual void OnCollision([[maybe_unused]] Collider* colliderGroup) {}

		//};
	}
}
