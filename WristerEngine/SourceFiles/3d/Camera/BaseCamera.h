#pragma once
#include "Matrix4.h"
#include "WindowsAPI.h"
#include "Shake.h"
#include <Transform.h>
#include <wrl.h>
#include <d3d12.h>

namespace WristerEngine::_3D
{
	// プロジェクション行列
	class Projection
	{
	private:
		Matrix4 matProjection;

	protected:
		// プロジェクション行列を更新
		void UpdateMatrix();

		// プロジェクション行列を取得
		const Matrix4& GetProjectionMatrix() const { return matProjection; }

	public:
		// 画角(度)
		Angle fovAngleY = 45;
		// アスペクト比(基本はWIN_SIZEに準拠)
		float aspectRatio = WIN_SIZE.x / WIN_SIZE.y;
		// 映る範囲
		float nearZ = 0.1f, farZ = 1000.0f;
	};

	// カメラ基底クラス
	class BaseCamera : Projection
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
		Matrix4 matView, matViewProjection;

		// シェイク値込みのカメラ座標
		Vector3 cameraPos;

		/// <summary>
		/// ビュー行列を更新
		/// </summary>
		/// <returns>シェイク値を含んだカメラの座標</returns>
		void UpdateViewMatrix();

	protected:
		// カメラ計算モード
		enum class CalMode
		{
			ETU, // eye, target, upで計算
			Transform // ワールド行列から計算
		};

		CalMode calMode = CalMode::ETU;

	public:
		ConstBufferData* constMap = nullptr;
		Microsoft::WRL::ComPtr<ID3D12Resource> constBuffer;
		Vector3 eye = { 0, 0, -50.0f }, target, up = AXIS_Y_3D;
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
		// ビュー行列
		const Matrix4& GetViewMatrix() const { return matView; }
		// ビュー行列とプロジェクション行列を掛け合わせた行列
		const Matrix4& GetViewProjectionMatrix() const { return matViewProjection; }
	};

	struct BaseCameraProp
	{
		virtual ~BaseCameraProp() = default;
	};
}