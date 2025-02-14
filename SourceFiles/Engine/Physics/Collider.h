#pragma once
#include "Transform.h"
#include "Physics.h"
#include <vector>
#include <array>
#include <memory>
#include "Sprite.h"
#include <map>

// コライダーの属性
enum class CollisionAttribute
{
	Player = 0b1,
	Enemy = 0b1 << 1,
	Goal = 0b1 << 2,
	All = -1
};

// コライダーのマスク
enum class CollisionMask
{
	None = 0,
	All = -1,
	Player = (int)CollisionAttribute::Enemy | (int)CollisionAttribute::Goal,
	Enemy = (int)CollisionAttribute::Player,
	Goal = (int)CollisionAttribute::Player
};

namespace WristerEngine
{
	namespace _2D
	{
		enum class CollisionShapeType
		{
			Unknown,
			Box,
			TwoRay
		};

		struct Option
		{
			Angle fov;
		};

		class ColliderGroup;

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

		class ColliderGroup
		{
		protected:
			std::list<std::unique_ptr<Base2DCollider>> colliders;
			CollisionAttribute collisionAttribute = CollisionAttribute::All;
			CollisionMask collisionMask = CollisionMask::All;
			std::map<size_t, std::vector<size_t>> collisionPair;

		public:
			// コンストラクタ
			ColliderGroup();
			// 仮想デストラクタ
			virtual ~ColliderGroup();

			// コライダーの追加
			void AddCollider(Sprite* transform, CollisionShapeType shapeType, const std::string& colliderName, const Option* option = nullptr);
			// コライダーの削除
			void DeleteCollider(const std::string& colliderName);
			// コリジョンペアの追加
			void AddCollisionPair(size_t myIndex, size_t youIndex);
			// コリジョンペアの追加
			void DeletePair();

			// getter
			CollisionAttribute GetCollisionAttribute() const { return collisionAttribute; }
			CollisionMask GetCollisionMask() const { return collisionMask; }
			const std::string GetColliderName(size_t index) const;
			const std::list<std::unique_ptr<Base2DCollider>>& GetColliders() const { return colliders; }

			// 衝突コールバック関数
			virtual void OnCollision([[maybe_unused]] ColliderGroup* colliderGroup) {}

		};
	}

	class BoxCollider;
	class SphereCollider;
	class PlaneCollider;
	class PolygonCollider;
	class RayCollider;
	class IncludeCollider;
	class MeshCollider;

	// コライダー基底クラス
	class BaseCollider
	{
	protected:
		CollisionAttribute collisionAttribute = CollisionAttribute::All;
		CollisionMask collisionMask = CollisionMask::All;
		_3D::Transform* worldTransform = nullptr;
		std::unique_ptr<Physics> physics;

	public:
		virtual ~BaseCollider() = default;

		// 衝突コールバック関数
		virtual void OnCollision([[maybe_unused]] BoxCollider* boxCollider) {}
		virtual void OnCollision([[maybe_unused]] SphereCollider* sphereCollider) {}
		virtual void OnCollision([[maybe_unused]] PlaneCollider* boxCollider) {}
		virtual void OnCollision([[maybe_unused]] PolygonCollider* sphereCollider) {}
		virtual void OnCollision([[maybe_unused]] RayCollider* sphereCollider) {}
		virtual void OnCollision([[maybe_unused]] IncludeCollider* sphereCollider) {}

		// setter
		void SetCollisionAttribute(CollisionAttribute collisionAttribute_) { collisionAttribute = collisionAttribute_; }
		void SetCollisionMask(CollisionMask collisionMask_) { collisionMask = collisionMask_; }
		void SetWorldTransform(_3D::Transform* worldTransform_) { worldTransform = worldTransform_; }
		// getter
		CollisionAttribute GetCollisionAttribute() { return collisionAttribute; }
		CollisionMask GetCollisionMask() { return collisionMask; }
		virtual Vector3 GetWorldPosition() { return worldTransform->GetWorldPosition(); }
		Physics* GetPhysics() { return physics.get(); }
	};

	// ボックスコライダー(AABB方式)
	class BoxCollider : public virtual BaseCollider
	{
	public:
		// コンストラクタ
		BoxCollider();
		// 仮想デストラクタ
		virtual ~BoxCollider();
		// 3軸方向の半径を取得
		virtual Vector3 GetRadius3D() { return worldTransform->scale; }
	};

	// 完全包含のボックスコライダー(AABB方式)
	class IncludeCollider : public virtual BaseCollider
	{
	public:
		enum class Axis { X, Y, Z };

	private:
		// 完全包含半径
		static float includeRadius;
		// 当たり判定を取るペアのtrueが少ないほうが計算に反映される
		std::array<bool, 3> isUseAxis = { true,true,true };

	public:
		// コンストラクタ
		IncludeCollider();
		// 仮想デストラクタ
		virtual ~IncludeCollider();
		// 完全包含半径の取得
		static float GetIncludeRadius() { return includeRadius; }
		// 使う軸の設定
		void SetUseAxis(Axis axis, bool isUse) { isUseAxis[(size_t)axis] = isUse; }
		// 使う軸の取得
		std::array<bool, 3> GetUseAxis() { return isUseAxis; }
	};

	// 球コライダー
	class SphereCollider : public virtual BaseCollider
	{
	public:
		// コンストラクタ
		SphereCollider();
		// 仮想デストラクタ
		virtual ~SphereCollider();
		// 半径取得
		virtual float GetRadius() { return worldTransform->scale.x; }
	};

	// 平面コライダー
	class PlaneCollider : public virtual BaseCollider
	{
	protected:
		// 基準法線
		Vector3 baseNormal = Vector3::MakeAxis(Axis::Z);
		float distance = 0;
		Vector3 inter;

	public:
		// コンストラクタ
		PlaneCollider();
		// 仮想デストラクタ
		virtual ~PlaneCollider();
		// setter
		void SetInter(const Vector3& inter_) { inter = inter_; }
		void SetDistance(float distance_) { distance = distance_; }
		void SetRotation(const Vector3& rotation) { worldTransform->rotation = rotation; }
		void SetBaseNormal(const Vector3& baseNormal_) { baseNormal = baseNormal_; }
		// getter
		virtual Vector3 GetNormal() { return baseNormal * Matrix4::Rotate(worldTransform->rotation); }
		virtual Vector3* GetInter() { return &inter; }
		virtual float GetDistance() { return distance; }
	};

	// 多角形平面コライダー
	class PolygonCollider : public virtual BaseCollider
	{
	protected:
		// 基準法線
		Vector3 baseNormal = Vector3::MakeAxis(Axis::Y);
		// 頂点は時計回り
		std::vector<Vector3> vertices;
		float distance = 0;
		// メッシュコライダーで使う
		Vector3 normal;

	public:
		// コンストラクタ
		PolygonCollider();
		// 仮想デストラクタ
		virtual ~PolygonCollider();
		// 頂点更新
		void UpdateVertices();
		// 距離を計算
		void ComputeDistance() { distance = Dot(GetNormal(), vertices[0]); }
		// 法線を計算
		void ComputeNormal();
		// 平面に変換する
		void ToPlaneCollider(PlaneCollider* planeCollider);
		// 頂点を追加
		void AddVertices(Vector3 pos) { vertices.push_back(pos); }
		// setter
		void SetBaseNormal(Vector3 baseNormal_) { baseNormal = baseNormal_; }
		virtual void SetVertices();
		// getter
		virtual Vector3 GetNormal() { return baseNormal * Matrix4::Rotate(worldTransform->rotation); }
		virtual std::vector<Vector3> GetVertices() { return vertices; }
	};

	// レイコライダー
	class RayCollider : public virtual BaseCollider
	{
	public:
		// 基準レイ
		Vector3 baseRayDirection = Vector3::MakeAxis(Axis::Z);
		// コンストラクタ
		RayCollider();
		// 仮想デストラクタ
		virtual ~RayCollider();
		// レイ方向を取得
		virtual const Vector3 GetRayDirection() { return baseRayDirection * Matrix4::Rotate(worldTransform->rotation); }
	};
}

// メッシュコライダー
//class MeshCollider : public BaseCollider
//{
//private:
//	// ワールド行列の逆行列
//	Matrix4 invMatWorld;
//
//public:
//	std::vector<PolygonCollider> triangles;
//	void ConstructTriangles(ModelManager* model);
//};