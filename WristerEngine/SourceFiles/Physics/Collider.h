#pragma once
#include "Transform.h"
#include "Physics.h"
#include <vector>
#include <array>
#include <memory>
#include "Sprite.h"
#include <map>
#include <ModelManager.h>

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

	enum class CollisionShapeType
	{
		Unknown = -1,
		Sphere,
		Box,
		IncludeBox,
		Plane,
		Ray,
		Mesh
	};

	struct CollisionInfo
	{
	protected:
		uint32_t attribute = 0;
		uint32_t mask = static_cast<uint32_t>(-1);

	public:
		// setter
		void SetAttribute(uint32_t attribute_) { attribute = attribute_; }
		void SetMask(uint32_t mask_) { mask = mask_; }
		// getter
		uint32_t GetAttribute() const { return attribute; }
		uint32_t GetMask() const { return mask; }
	};

	class Collider;

	// コライダー基底クラス
	class BaseCollider : public CollisionInfo
	{
	private:
		std::unique_ptr<Physics> physics;
		bool isDestroy = false;
		Collider* owner = nullptr;

	protected:
		_3D::Object3d* debugObject = nullptr;
		CollisionShapeType shapeType = CollisionShapeType::Unknown;

	public:
		virtual ~BaseCollider() = default;

		virtual void Update() {}

		void Destroy() { isDestroy = true; }

		void SetOwner(Collider* owner_) { owner = owner_; }

		// getter
		Physics* GetPhysics() { return physics.get(); }
		CollisionShapeType GetShapeType() const { return shapeType; }
		bool IsDestroy() const { return isDestroy; }
		Collider* GetOwner() { return owner; }
	};

	class ColliderGroup : public CollisionInfo
	{
	private:
		std::list<std::unique_ptr<BaseCollider>> colliders;

		/// <summary>
		/// 当たったペアの記録
		/// </summary>
		/// <param name="first">自分のコライダー</param>
		/// <param name="second">相手のコライダー</param>
		std::vector<std::pair<BaseCollider*, BaseCollider*>> collisionPair;

	public:
		~ColliderGroup() { colliders.clear(); }

		void Update();

		/// <summary>
		/// コライダーを登録
		/// </summary>
		/// <param name="shapeType">コライダーの形状</param>
		/// <returns>登録されたコライダー</returns>
		BaseCollider* AddCollider(CollisionShapeType shapeType, Collider* owner);

		void AddCollisionPair(BaseCollider* colliderA, BaseCollider* colliderB);

		// getter
		const std::list<std::unique_ptr<BaseCollider>>* GetColliders() const { return &colliders; }
		const std::vector<std::pair<BaseCollider*, BaseCollider*>>& GetCollisionPair() const { return collisionPair; }
	};

	class Collider
	{
	protected:
		ColliderGroup* group = nullptr;

		void Initialize(const std::string& groupName);

	public:
		// getter
		const std::vector<std::pair<BaseCollider*, BaseCollider*>>& GetCollisionPair() const { return group->GetCollisionPair(); }
		ColliderGroup* GetGroup() const { return group; }

		// 衝突コールバック関数
		virtual void OnCollision() {}
	};

	// 球コライダー
	class SphereCollider : public BaseCollider
	{
	private:
		Vector3 center;			// 中心座標
		float radius = 1.0f;	// 半径
		const _3D::Transform* pTransform = nullptr;

	public:
		SphereCollider() { shapeType = CollisionShapeType::Sphere; }
		void Update() override { if (pTransform) { center = pTransform->GetWorldPosition(); } }
		// 中心座標を取得
		const Vector3& GetCenterPosition() const { return center; }
		// 半径を取得
		float GetRadius() const { return radius; }
		// トランスフォームを設定
		void SetTransform(_3D::Transform* pTransform_) { pTransform = pTransform_; }
		// 中心座標を設定
		void SetCenterPosition(const Vector3& center_) { center = center_; }
		// 半径を設定
		void SetRadius(float radius_) { radius = radius_; }
	};

	// ボックスコライダー(AABB方式)
	class BoxCollider : public BaseCollider
	{
	private:
		Vector3 center;				// 中心座標
		Vector3 radius = { 1,1,1 };	// 各軸方向の半径

	public:
		// コンストラクタ
		BoxCollider() { shapeType = CollisionShapeType::Box; }
		// 中心座標を取得
		const Vector3& GetCenterPosition() const { return center; }
		// 3軸方向の半径を取得
		const Vector3& GetRadius() const { return radius; }
		// 中心座標を設定
		void SetCenterPosition(const Vector3& center_) { center = center_; }
		// 3軸方向の半径を設定
		void SetRadius(const Vector3& radius_) { radius = radius_; }
	};

	// 完全包含のボックスコライダー(AABB方式)
	class IncludeCollider : public BaseCollider
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
		IncludeCollider() { shapeType = CollisionShapeType::IncludeBox; }
		// 完全包含半径の取得
		static float GetIncludeRadius() { return includeRadius; }
		// 使う軸の設定
		void SetUseAxis(Axis axis, bool isUse) { isUseAxis[(size_t)axis] = isUse; }
		// 使う軸の取得
		std::array<bool, 3> GetUseAxis() { return isUseAxis; }
	};

	// 平面コライダー
	class PlaneCollider : public BaseCollider
	{
	protected:
		// 法線ベクトル
		Vector3 normal = Vector3::MakeAxis(Axis::Y);
		// 原点(0,0,0)からの距離
		float distance = 0;
		Vector3 inter;

	public:
		// コンストラクタ
		PlaneCollider() { shapeType = CollisionShapeType::Plane; }
		// setter
		void SetInter(const Vector3& inter_) { inter = inter_; }
		void SetDistance(float distance_) { distance = distance_; }
		//void SetRotation(const Vector3& rotation) { pTransform->rotation = rotation; }
		void SetBaseNormal(const Vector3& normal_) { normal = normal_; }
		// getter
		//Vector3 GetNormal() { return baseNormal * Matrix4::Rotate(pTransform->rotation); }
		Vector3* GetInter() { return &inter; }
		float GetDistance() { return distance; }
	};

	//// 多角形平面コライダー
	//class PolygonCollider : public virtual BaseCollider
	//{
	//protected:
	//	// 基準法線
	//	Vector3 baseNormal = Vector3::MakeAxis(Axis::Y);
	//	// 頂点は時計回り
	//	std::vector<Vector3> vertices;
	//	float distance = 0;
	//	// メッシュコライダーで使う
	//	Vector3 normal;
	//
	//public:
	//	// コンストラクタ
	//	PolygonCollider();
	//	// 頂点更新
	//	void UpdateVertices();
	//	// 距離を計算
	//	void ComputeDistance() { distance = Dot(GetNormal(), vertices[0]); }
	//	// 法線を計算
	//	void ComputeNormal();
	//	// 平面に変換する
	//	void ToPlaneCollider(PlaneCollider* planeCollider);
	//	// 頂点を追加
	//	void AddVertices(Vector3 pos) { vertices.push_back(pos); }
	//	// setter
	//	void SetBaseNormal(Vector3 baseNormal_) { baseNormal = baseNormal_; }
	//	virtual void SetVertices();
	//	// getter
	//	virtual Vector3 GetNormal() { return baseNormal * Matrix4::Rotate(pTransform->rotation); }
	//	virtual std::vector<Vector3> GetVertices() { return vertices; }
	//};

	// レイコライダー
	class RayCollider : public BaseCollider
	{
	public:
		// 基準レイ
		Vector3 baseRayDirection = Vector3::MakeAxis(Axis::Z);
		// コンストラクタ
		RayCollider() { shapeType = CollisionShapeType::Ray; }
		// レイ方向を取得
		//virtual const Vector3 GetRayDirection() { return baseRayDirection * Matrix4::Rotate(pTransform->rotation); }
	};

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
}
