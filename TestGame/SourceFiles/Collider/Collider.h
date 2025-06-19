#pragma once
#include "Physics.h"
#include "Sprite.h"
#include <map>
#include <ModelManager.h>
#include <optional>

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
		Triangle,
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
		uint32_t serialNumber = 0;
		static uint32_t nextSerialNumber;

	protected:
		CollisionShapeType shapeType = CollisionShapeType::Unknown;
		const _3D::Transform* pTransform = nullptr;

	public:
		BaseCollider(bool isDec = false) { serialNumber = nextSerialNumber++; if (isDec) { nextSerialNumber--; } }

		virtual ~BaseCollider() = default;

		virtual void Update() {}

		void Destroy() { isDestroy = true; }

		void SetOwner(Collider* owner_) { owner = owner_; }
		// トランスフォームを設定
		void SetTransform(const _3D::Transform* pTransform_) { pTransform = pTransform_; Update(); }

		// getter
		Physics* GetPhysics() { return physics.get(); }
		CollisionShapeType GetShapeType() const { return shapeType; }
		bool IsDestroy() const { return isDestroy; }
		Collider* GetOwner() { return owner; }
		uint32_t GetSerialNumber() const { return serialNumber; }
	};

	struct CollisionPair
	{
		BaseCollider* my = nullptr, * other = nullptr;
		std::optional<Vector3> inter;
		std::optional<float> distance = std::nullopt;

		CollisionPair(BaseCollider* my, BaseCollider* other,
			const std::optional<Vector3>& inter, std::optional<float> distance);

		static bool Check(const CollisionPair& p1, const CollisionPair& p2);
	};

	class ColliderGroup : public CollisionInfo
	{
	private:
		std::list<std::unique_ptr<BaseCollider>> colliders;
		std::list<Collider*> owners;

		// 当たったペアの記録
		std::vector<CollisionPair> collisionPairs;
		std::vector<CollisionPair> enterPairs;
		std::vector<CollisionPair> exitPairs;
		std::vector<CollisionPair> collisionPairsPre;

	public:
		~ColliderGroup();

		void Update();

		/// <summary>
		/// コライダーを登録
		/// </summary>
		/// <param name="shapeType">コライダーの形状</param>
		/// <returns>登録されたコライダー</returns>
		BaseCollider* AddCollider(std::unique_ptr<BaseCollider> newCollider);

		void AddCollisionPair(const CollisionPair& pair);

		void AddOwner(Collider* owner) { owners.push_back(owner); }

		void CallCollision();

		// getter
		const std::list<std::unique_ptr<BaseCollider>>* GetColliders() const { return &colliders; }
		const std::vector<CollisionPair>& GetCollisionPairs() const { return collisionPairs; }
		const std::vector<CollisionPair>& GetEnterPairs() const { return enterPairs; }
		const std::vector<CollisionPair>& GetExitPairs() const { return exitPairs; }
	};

	class Collider
	{
	private:
		uint32_t serialNumber = 0;
		static uint32_t nextSerialNumber;

	protected:
		ColliderGroup* group = nullptr;

		void Initialize(const std::string& groupName);

		/// <summary>
		/// コライダーを登録
		/// </summary>
		/// <param name="shapeType">コライダーの形状</param>
		/// <returns>登録されたコライダー</returns>
		BaseCollider* AddCollider(CollisionShapeType shapeType);

	public:
		Collider() { serialNumber = nextSerialNumber++; }
		virtual ~Collider();

		void DeleteGroup() { group = nullptr; }

		// getter
		const std::vector<CollisionPair>& GetCollisionPairs() const { return group->GetCollisionPairs(); }
		ColliderGroup* GetGroup() const { return group; }
		uint32_t GetSerialNumber() const { return serialNumber; }

		// 衝突コールバック関数
		// 当たっている間
		virtual void OnCollision() {}
		// 当たった瞬間
		virtual void OnCollisionEnter() {}
		// 離れた瞬間
		virtual void OnCollisionExit() {}
	};

	// 球コライダー
	class SphereCollider : public BaseCollider
	{
	private:
		Vector3 center;			// 中心座標
		float radius = 1.0f;	// 半径
		Vector3 offset;

		void Update() override { if (pTransform) { center = pTransform->GetWorldPosition() + offset; } }

	public:
		SphereCollider(bool isDec = false) : BaseCollider(isDec) { shapeType = CollisionShapeType::Sphere; }
		// 中心座標を取得
		const Vector3& GetCenterPosition() const { return center; }
		// 半径を取得
		float GetRadius() const { return radius; }
		// 中心座標を設定
		void SetCenterPosition(const Vector3& center_) { center = center_ + offset; }
		// オフセットを設定
		void SetOffset(const Vector3& offset_) { offset = offset_; }
		// 半径を設定
		void SetRadius(float radius_) { radius = radius_; }
	};

	// ボックスコライダー(AABB方式)
	class BoxCollider : public BaseCollider
	{
	private:
		Vector3 center;				// 中心座標
		Vector3 radius = { 1,1,1 };	// 各軸方向の半径
		Vector3 offset;

	public:
		// コンストラクタ
		BoxCollider(bool isDec = false) : BaseCollider(isDec) { shapeType = CollisionShapeType::Box; }
		// 中心座標を取得
		const Vector3& GetCenterPosition() const { return center; }
		// 3軸方向の半径を取得
		const Vector3& GetRadius() const { return radius; }
		// 中心座標を設定
		void SetCenterPosition(const Vector3& center_) { center = center_; }
		// オフセットを設定
		void SetOffset(const Vector3& offset_) { offset = offset_; }
		// 3軸方向の半径を設定
		void SetRadius(const Vector3& radius_) { radius = radius_; }
	};

	// 完全包含のボックスコライダー(AABB方式)
	class IncludeCollider : public BaseCollider
	{
	public:
		enum class Axis { X, Y, Z };

	private:
		Vector3 center;		// 中心座標
		// 完全包含半径
		static float includeRadius;
		// 当たり判定を取るペアのtrueが少ないほうが計算に反映される
		std::array<bool, 3> isUseAxis = { true,true,true };

	public:
		// コンストラクタ
		IncludeCollider() { shapeType = CollisionShapeType::IncludeBox; }
		// 中心座標を取得
		const Vector3& GetCenterPosition() const { return center; }
		// 完全包含半径の取得
		static float GetIncludeRadius() { return includeRadius; }
		// 使う軸の取得
		std::array<bool, 3> GetUseAxis() const { return isUseAxis; }
		// 中心座標を設定
		void SetCenterPosition(const Vector3& center_) { center = center_; }
		// 使う軸の設定
		void SetUseAxis(Axis axis, bool isUse) { isUseAxis[(size_t)axis] = isUse; }
	};

	// 三角形コライダー
	class TriangleCollider : public BaseCollider
	{
	private:
		// トランスフォーム未適用時の頂点座標
		std::array<Vector3, 3> initV;

	protected:
		// 頂点座標3つ
		std::array<Vector3, 3> vertices;
		// 法線ベクトル
		Vector3 normal = Vector3::MakeAxis(Axis::Y);
		// 基準法線(トランスフォームから計算する場合)
		Vector3 baseNormal = Vector3::MakeAxis(Axis::Y);
		// 頂点更新
		void Update() override;

	public:
		// コンストラクタ
		TriangleCollider(bool isDec = false) : BaseCollider(isDec) { shapeType = CollisionShapeType::Triangle; }
		void ComputeNormal();
		// setter
		void SetVertices(const std::array<Vector3, 3>& vertices_) { vertices = initV = vertices_; }
		void SetNormal(const Vector3& normal_) { normal = Normalize(normal_); }
		void SetBaseNormal(const Vector3& normal_) { baseNormal = Normalize(normal_); }
		// getter
		Vector3 GetNormal() const { return normal; }
		const std::array<Vector3, 3>& GetVertices() const { return vertices; }
	};

	// 平面コライダー
	class PlaneCollider : public BaseCollider
	{
	protected:
		// 法線ベクトル
		Vector3 normal = Vector3::MakeAxis(Axis::Y);
		// 原点(0,0,0)からの距離
		float distance = 0;
		// 基準法線(トランスフォームから計算する場合)
		Vector3 baseNormal = Vector3::MakeAxis(Axis::Y);

		void Update() override;

	public:
		// コンストラクタ
		PlaneCollider() { shapeType = CollisionShapeType::Plane; }
		PlaneCollider(const TriangleCollider& triangle);
		// setter
		void SetDistance(float distance_) { distance = distance_; }
		void SetNormal(const Vector3& normal_) { normal = Normalize(normal_); }
		void SetBaseNormal(const Vector3& normal_) { baseNormal = Normalize(normal_); }
		// getter
		const Vector3& GetNormal() const { return normal; }
		float GetDistance() const { return distance; }
	};

	// レイコライダー
	class RayCollider : public BaseCollider
	{
	private:
		// 始点座標
		Vector3 start;
		// 方向
		Vector3 dir = Vector3::MakeAxis(Axis::Z);
		// 基準レイ
		Vector3 baseDir = Vector3::MakeAxis(Axis::Z);

		void Update() override;

	public:
		// コンストラクタ
		RayCollider(bool isDec = false) : BaseCollider(isDec) { shapeType = CollisionShapeType::Ray; }
		const Vector3& GetStartPos() const { return start; }
		const Vector3& GetDir() const { return dir; }
		void SetStartPos(const Vector3& start_) { start = start_; }
		void SetDir(const Vector3& dir_) { dir = Normalize(dir_); }
		void SetBaseDir(const Vector3& dir_) { baseDir = Normalize(dir_); }
	};

	// メッシュコライダー
	class MeshCollider : public BaseCollider
	{
	private:
		std::list<std::unique_ptr<TriangleCollider>> triangles;
		// ワールド行列の逆行列
		Matrix4 invMatWorld;

		void Update() override;

	public:
		MeshCollider() { shapeType = CollisionShapeType::Mesh; }
		void ConstructTriangles(const _3D::Mesh* mesh);
		const Matrix4& GetInvMatWorld() const { return invMatWorld; }
		const std::list<std::unique_ptr<TriangleCollider>>& GetTriangles() const { return triangles; }
		const Matrix4& GetMatWorld() const { return pTransform->matWorld; }
	};
}
