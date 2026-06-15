#pragma once
#include "Transform.h"
#include <map>
#include <optional>
#include "CollisionInfo.h"

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

	class BaseCollider : public ColliderInfo
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
		BaseCollider(const ColliderInfo& info) : ColliderInfo(info) { serialNumber = nextSerialNumber++; }

		virtual ~BaseCollider() = default;

		virtual void Update() {}

		void Destroy() { isDestroy = true; }

		void SetOwner(Collider* owner_) { owner = owner_; }
		// トランスフォームを設定
		void SetTransform(const Transform* pTransform_) { pTransform = pTransform_; Update(); }

		// getter
		CollisionShapeType GetShapeType() const { return shapeType; }
		bool IsDestroy() const { return isDestroy; }
		Collider* GetOwner() { return owner; }
		uint32_t GetSerialNumber() const { return serialNumber; }
	};

	struct HitInfo
	{
		std::optional<Vector2> inter = std::nullopt;
		std::optional<float> distance = std::nullopt;
		std::optional<Vector2> reject = std::nullopt;

		HitInfo(const std::optional<Vector2>& inter = std::nullopt, const std::optional<float>& distance = std::nullopt,
			const std::optional<Vector2>& reject = std::nullopt)
			: inter(inter), distance(distance), reject(reject)
		{
		}

		void Reset() { inter = reject = std::nullopt; distance = std::nullopt; }
	};

	struct CollisionPair : public HitInfo
	{
		BaseCollider* my = nullptr, * other = nullptr;

		CollisionPair(BaseCollider* my, BaseCollider* other, const HitInfo& hitInfo);

		static bool Check(const CollisionPair& p1, const CollisionPair& p2);
	};

	class ColliderGroup : public ColliderInfo
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

	// 四角形コライダー
	class BoxCollider : public BaseCollider
	{
	public:
		BoxCollider(const ColliderInfo& info) : BaseCollider(info) { shapeType = CollisionShapeType::Box; }
		// 左上端と右下端の座標を求める
		std::map<std::string, Vector2> GetVertex() const;
	};

	// 1点から2方向に延びる線分との当たり判定（まだ不完全）
	class TwoRayCollider : public BaseCollider
	{
		Angle fov; // 視野角

	public:
		TwoRayCollider(const ColliderInfo& info) : BaseCollider(info) { shapeType = CollisionShapeType::TwoRay; }
		Angle GetFOV() const { return fov; }
	};

	class CircleCollider;

	// 円コライダー
	class CircleCollider : public BaseCollider
	{
	private:
		Vector2 center;			// 中心座標
		float radius = 50.0f;	// 半径(ピクセル)
		Vector2 offset;			// 中心座標のオフセット(トランスフォームからの差分)

	public:
		CircleCollider(const ColliderInfo& info) : BaseCollider(info) { shapeType = CollisionShapeType::Circle; }
		void Update() override { if (pTransform) { center = pTransform->GetWorldPosition() + offset; } }
		// 中心座標を取得
		const Vector2& GetCenterPosition() const { return center; }
		// 半径を取得
		float GetRadius() const { return radius; }
		// 中心座標を設定
		void SetCenterPosition(const Vector2& center_) { center = center_ + offset; }
		// オフセットを設定
		void SetOffset(const Vector2& offset_) { offset = offset_; }
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
			std::unique_ptr<BaseCollider> newCollider;
			const std::string TYPE_NAME = typeid(T).name();
			ColliderInfo defaultInfo = { group->GetAttribute(), group->GetMask(), group->GetName() };
			if (info) { defaultInfo = *info; }

			auto TypeCompare = [&TYPE_NAME](const std::string& type) { return TYPE_NAME.find(type) != std::string::npos; };

			if (TypeCompare("Circle")) { newCollider = std::make_unique<CircleCollider>(defaultInfo); }
			else if (TypeCompare("Box")) { newCollider = std::make_unique<BoxCollider>(defaultInfo); }
			else if (TypeCompare("TwoRay")) { newCollider = std::make_unique<TwoRayCollider>(defaultInfo); }

			newCollider->SetOwner(this);

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