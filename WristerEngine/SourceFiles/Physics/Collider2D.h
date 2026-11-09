#pragma once
#include "Transform.h"
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

	class ColliderFactory : public IColliderFactory
	{
	public:
		uPtr<BaseSingleCollider> CreateCollider(uint32_t colliderShapeType) override;
	};

	struct HitInfo2D : public HitInfo
	{
		std::optional<Vector2> inter = std::nullopt;
		std::optional<Vector2> reject = std::nullopt;

		~HitInfo2D() = default;
	};

	class SingleCollider : public BaseSingleCollider
	{

	protected:
		const Transform* pTransform = nullptr;

	public:
		virtual ~SingleCollider() = default;
		
		// トランスフォームを設定
		void SetTransform(const Transform* pTransform_) { pTransform = pTransform_; Update(); }

		const Transform* GetTransform() const { return pTransform; }
	};

	// 四角形コライダー
	class BoxCollider : public SingleCollider
	{
	public:
		BoxCollider() : SingleCollider() { shapeType = (uint32_t)CollisionShapeType::Box; }
		// 左上端と右下端の座標を求める
		std::map<std::string, Vector2> GetVertex() const;
	};

	// 1点から2方向に延びる線分との当たり判定（まだ不完全）
	class TwoRayCollider : public SingleCollider
	{
		Angle fov; // 視野角

	public:
		TwoRayCollider() : SingleCollider() { shapeType = (uint32_t)CollisionShapeType::TwoRay; }
		Angle GetFOV() const { return fov; }
	};

	// 円コライダー
	class CircleCollider : public SingleCollider
	{
	private:
		Vector2 center;			// 中心座標
		float radius = 50.0f;	// 半径(ピクセル)
		Vector2 offset;			// 中心座標のオフセット(トランスフォームからの差分)

	public:
		CircleCollider() : SingleCollider() { shapeType = (uint32_t)CollisionShapeType::Circle; }
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

	class Collider : public BaseCollider
	{
	public:
		// BaseCollider を介して継承されました
		ColliderGroup* Initialize(CR<std::string> groupName, const std::optional<ColliderInfo>& info) override;
	};

	// 個別当たり判定
	bool Check2Circles(const CircleCollider* a, const CircleCollider* b);
	bool Check2Boxes(const std::array<_2D::BoxCollider*, 2>& colliders);
	//bool CheckBox2Rays(const std::array<_2D::TwoRayCollider*, 2>& colliders);
}