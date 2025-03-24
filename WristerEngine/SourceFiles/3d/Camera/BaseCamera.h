#pragma once
#include "Matrix4.h"
#include "WindowsAPI.h"
#include "Shake.h"
#include <Transform.h>
#include <wrl.h>
#include <d3d12.h>

namespace WristerEngine::_3D
{
	// カメラ基底クラス
	class BaseCamera
	{
	private:
		// 定数バッファ用データ構造体
		struct ConstBufferData
		{
			Matrix4 viewproj; // ビュープロジェクション行列
			Vector3 cameraPos; // カメラ座標(ワールド座標)
		};

		// ワールド行列を使った計算を行うためのTransformポインタ
		const Transform* pTransform = nullptr;
		Matrix4 matView, matProjection, matViewProjection;

		/// <summary>
		/// ビュー行列を更新
		/// </summary>
		/// <returns>シェイク値を含んだカメラの座標</returns>
		Vector3 UpdateViewMatrix();

		// プロジェクション行列を更新
		void UpdateProjectionMatrix();

	public:
		ConstBufferData* constMap = nullptr;
		Microsoft::WRL::ComPtr<ID3D12Resource> constBuffer;
		Vector3 eye = { 0, 0, -50.0f }, target, up = Vector3::MakeAxis(Axis::Y);
		Angle fovAngleY = 45; // 画角(度)
		// アスペクト比(基本はWIN_SIZEに準拠)
		float aspectRatio = WIN_SIZE.x / WIN_SIZE.y;
		float nearZ = 0.1f, farZ = 1000.0f; // 映る範囲
		// シェイク機能
		std::unique_ptr<Shake> shake;

		// 仮想デストラクタ
		virtual ~BaseCamera() = default;
		// 初期化
		void Initialize();
		// 更新
		virtual void Update();
		// 固有の更新処理
		virtual void VirtualUpdate() {}
		// シェイクを生成する
		void CreateShake(const Shake::Prop& shakeProp);
		// カメラ移動
		void CameraMove(const Vector3& move);
		// ワールド行列をセット
		void SetTransform(const Transform* transform) { pTransform = transform; }
		// カメラに対してのビルボード行列を返す
		Matrix4 GetBillboard() const;
		// ワールド行列をセット
		const Transform* GetTransform() const { return pTransform; }
		// ビュー行列とプロジェクション行列を掛け合わせた行列
		const Matrix4& GetViewProjectionMatrix() const { return matViewProjection; }
	};
}