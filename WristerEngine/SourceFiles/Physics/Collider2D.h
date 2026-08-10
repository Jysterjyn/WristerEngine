#pragma once
#include "Transform.h"
#include <map>
#include "ColliderBase.h"

namespace WristerEngine::_2D
{
	enum class CollisionShapeType
	{
		Unknown,
		Box,
		TwoRay,
		Circle
	};

	class Collider;

	class BaseCollider2D : public ColliderInfo
	{
	private:
		bool isDestroy = false;
		Collider* owner = nullptr;
		uint32_t serialNumber = 0;
		static uint32_t nextSerialNumber;

	protected:
		CollisionShapeType shapeType = CollisionShapeType::Unknown;
		const Transform* pTransform = nullptr;

	public:
		BaseCollider2D() { serialNumber = nextSerialNumber++; }

		virtual ~BaseCollider2D() = default;

		virtual void Update() {}

		void Destroy() { isDestroy = true; }

		void Initialize(Collider* owner_, const ColliderInfo& info);
		// トランスフォームを設定
		void SetTransform(const Transform* pTransform_) { pTransform = pTransform_; Update(); }

		// getter
		CollisionShapeType GetShapeType() const { return shapeType; }
		bool IsDestroy() const { return isDestroy; }
		Collider* GetOwner() { return owner; }
		uint32_t GetSerialNumber() const { return serialNumber; }
	};

	struct CollisionPair : public HitInfo<Vector2>
	{
		BaseCollider2D* my = nullptr, * other = nullptr;

		CollisionPair(BaseCollider2D* my, BaseCollider2D* other, const HitInfo& hitInfo);

		static bool Check(const CollisionPair& p1, const CollisionPair& p2);
	};

	class ColliderGroup : public ColliderInfo
	{
	private:
		std::list<std::unique_ptr<BaseCollider2D>> colliders;
		std::list<Collider*> owners;

		// 当たったペアの記録
		std::vector<CollisionPair> collisionPairs;
		std::vector<CollisionPair> enterPairs;
		std::vector<CollisionPair> exitPairs;
		std::vector<CollisionPair> collisionPairsPre;

	public:
		ColliderGroup(const std::string& groupName) { SetName(groupName); }
		~ColliderGroup();

		void Update();

		/// <summary>
		/// コライダーを登録
		/// </summary>
		/// <param name="shapeType">コライダーの形状</param>
		/// <returns>登録されたコライダー</returns>
		BaseCollider2D* AddCollider(std::unique_ptr<BaseCollider2D> newCollider);

		void AddCollisionPair(const CollisionPair& pair);

		void AddOwner(Collider* owner) { owners.push_back(owner); }

		void CallCollision();

		// getter
		const std::list<std::unique_ptr<BaseCollider2D>>* GetColliders() const { return &colliders; }
		const std::vector<CollisionPair>& GetCollisionPairs() const { return collisionPairs; }
		const std::vector<CollisionPair>& GetEnterPairs() const { return enterPairs; }
		const std::vector<CollisionPair>& GetExitPairs() const { return exitPairs; }
	};

	// 四角形コライダー
	class BoxCollider : public BaseCollider2D
	{
	public:
		BoxCollider() : BaseCollider2D() { shapeType = CollisionShapeType::Box; }
		// 左上端と右下端の座標を求める
		std::map<std::string, Vector2> GetVertex() const;
	};

	// 1点から2方向に延びる線分との当たり判定（まだ不完全）
	class TwoRayCollider : public BaseCollider2D
	{
		Angle fov; // 視野角

	public:
		TwoRayCollider() : BaseCollider2D() { shapeType = CollisionShapeType::TwoRay; }
		Angle GetFOV() const { return fov; }
	};

	class CircleCollider;

	// 円コライダー
	class CircleCollider : public BaseCollider2D
	{
	private:
		Vector2 center;			// 中心座標
		float radius = 50.0f;	// 半径(ピクセル)
		Vector2 offset;			// 中心座標のオフセット(トランスフォームからの差分)

	public:
		CircleCollider() : BaseCollider2D() { shapeType = CollisionShapeType::Circle; }
		void Update() override { if (pTransform) { center = pTransform->GetWorldPosition() + offset; } }
		// 中心座標を取得
		CR<Vector2> GetCenterPosition() const { return center; }
		// 半径を取得
		float GetRadius() const { return radius; }
		// 中心座標を設定
		void SetCenterPosition(CR<Vector2> center_) { center = center_ + offset; }
		// オフセットを設定
		void SetOffset(CR<Vector2> offset_) { offset = offset_; }
		// 半径を設定
		void SetRadius(float radius_) { radius = radius_; }
	};

	class Collider
	{
	private:
		uint32_t serialNumber = 0;
		static uint32_t nextSerialNumber;

	protected:
		ColliderGroup* group = nullptr;

		void Initialize(const std::string& groupName, const std::optional<ColliderInfo>& info = std::nullopt);

		/// <summary>
		/// コライダーを登録
		/// </summary>
		/// <param name="shapeType">コライダーの形状</param>
		/// <returns>登録されたコライダー</returns>
		template<class T>
		T* AddCollider(const std::optional<ColliderInfo>& info = std::nullopt)
		{
			std::unique_ptr<BaseCollider2D> newCollider;
			const std::string TYPE_NAME = typeid(T).name();
			ColliderInfo colliderInfo(group->GetColliderInfo());
			std::string groupName = info ? info->GetName() : group->GetName();
			colliderInfo.SetName(groupName);

			auto TypeCompare = [&TYPE_NAME](const std::string& type) { return TYPE_NAME.find(type) != std::string::npos; };
			if (!TypeCompare("Collider")) { return nullptr; }
			newCollider = std::make_unique<T>();

			//if (TypeCompare("Circle")) { newCollider = std::make_unique<CircleCollider>(); }
			//else if (TypeCompare("Box")) { newCollider = std::make_unique<BoxCollider>(); }
			//else if (TypeCompare("TwoRay")) { newCollider = std::make_unique<TwoRayCollider>(); }
			//if (!newCollider) { return nullptr; }

			newCollider->Initialize(this, colliderInfo);

			return static_cast<T*>(group->AddCollider(std::move(newCollider)));
		}

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

	// 個別当たり判定
	bool Check2Circles(const CircleCollider* a, const CircleCollider* b);
}