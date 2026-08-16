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

	class SingleCollider2D : public BaseSingleCollider<Vector2>
	{
	private:
		static uint32_t nextSerialNumber;

	protected:
		CollisionShapeType shapeType = CollisionShapeType::Unknown;
		const Transform* pTransform = nullptr;

	public:
		SingleCollider2D() { serialNumber = nextSerialNumber++; }
		virtual ~SingleCollider2D() = default;
		
		// トランスフォームを設定
		void SetTransform(const Transform* pTransform_) { pTransform = pTransform_; Update(); }

		// getter
		CollisionShapeType GetShapeType() const { return shapeType; }
	};

	class ColliderGroup : public BaseColliderGroup<Vector2>
	{
	private:

	public:
		~ColliderGroup();

		void Update();

		/// <summary>
		/// コライダーを登録
		/// </summary>
		/// <param name="shapeType">コライダーの形状</param>
		/// <returns>登録されたコライダー</returns>
		SingleCollider2D* AddCollider(std::unique_ptr<SingleCollider2D> newCollider);

		void AddCollisionPair(const CollisionPair& pair);

		void AddOwner(Collider* owner) { owners.push_back(owner); }

		void CallCollision();

		// getter
		const std::list<std::unique_ptr<SingleCollider2D>>* GetColliders() const { return &colliders; }
		const std::vector<CollisionPair>& GetCollisionPairs() const { return collisionPairs; }
		const std::vector<CollisionPair>& GetEnterPairs() const { return enterPairs; }
		const std::vector<CollisionPair>& GetExitPairs() const { return exitPairs; }
	};

	// 四角形コライダー
	class BoxCollider : public SingleCollider2D
	{
	public:
		BoxCollider() : SingleCollider2D() { shapeType = CollisionShapeType::Box; }
		// 左上端と右下端の座標を求める
		std::map<std::string, Vector2> GetVertex() const;
	};

	// 1点から2方向に延びる線分との当たり判定（まだ不完全）
	class TwoRayCollider : public SingleCollider2D
	{
		Angle fov; // 視野角

	public:
		TwoRayCollider() : SingleCollider2D() { shapeType = CollisionShapeType::TwoRay; }
		Angle GetFOV() const { return fov; }
	};

	// 円コライダー
	class CircleCollider : public SingleCollider2D
	{
	private:
		Vector2 center;			// 中心座標
		float radius = 50.0f;	// 半径(ピクセル)
		Vector2 offset;			// 中心座標のオフセット(トランスフォームからの差分)

	public:
		CircleCollider() : SingleCollider2D() { shapeType = CollisionShapeType::Circle; }
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

	class Collider : public BaseCollider<Vector2>
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
			std::unique_ptr<SingleCollider2D> newCollider;
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
		virtual ~Collider() = default;
	};

	// 個別当たり判定
	bool Check2Circles(const CircleCollider* a, const CircleCollider* b);
}