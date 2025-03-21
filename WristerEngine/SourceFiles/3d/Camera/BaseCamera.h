#pragma once
#include "Matrix4.h"
#include "WindowsAPI.h"
#include "Shake.h"
#include <wrl.h>
#include <d3d12.h>

namespace WristerEngine::_3D
{
	// カメラ基底クラス
	class BaseCamera
	{
	protected:
		// 定数バッファ用データ構造体
		struct ConstBufferData
		{
			Matrix4 viewproj; // ビュープロジェクション行列
			Vector3 cameraPos; // カメラ座標(ワールド座標)
		};

		// シェイク込みの値
		Vector3 sTarget;
		Vector3 sEye;

	public:
		ConstBufferData* constMap = nullptr;
		Microsoft::WRL::ComPtr<ID3D12Resource> constBuffer;
		Vector3 eye = { 0, 0, -50.0f }, target, up = Vector3::MakeAxis(Axis::Y);
		Angle fovAngleY = 45; // 画角(度)
		// アスペクト比(基本はWIN_SIZEに準拠)
		float aspectRatio = WIN_SIZE.x / WIN_SIZE.y;
		float nearZ = 0.1f, farZ = 1000.0f; // 映る範囲
		Matrix4 matView, matProjection, matViewProjection;
		// シェイク機能
		std::unique_ptr<Shake> shake;

		// 仮想デストラクタ
		virtual ~BaseCamera() = default;
		// 初期化
		void Initialize();
		// 更新
		void Update();
		// 固有の更新処理
		virtual void VirtualUpdate() = 0;
		// シェイクを生成する
		void CreateShake(const Shake::Prop& shakeProp);
		// カメラ移動
		void CameraMove(const Vector3& move);
		// ビュー行列とプロジェクション行列を掛け合わせた行列
		const Matrix4& GetViewProjectionMatrix() const { return matViewProjection; }

	private:
		// プロジェクション行列を求める
		void UpdateProjectionMatrix();
	};
}