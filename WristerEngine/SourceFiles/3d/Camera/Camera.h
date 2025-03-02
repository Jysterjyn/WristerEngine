#pragma once
#include "Matrix4.h"
#include "WindowsAPI.h"
#include "CameraShake.h"
#include <wrl.h>
#include <d3d12.h>

namespace WristerEngine::_3D
{
	// カメラ
	class Camera
	{
	private:
		Matrix4 matView, matProjection, matViewProjection;

		// 定数バッファ用データ構造体
		struct ConstBufferData
		{
			Matrix4 viewproj; // ビュープロジェクション行列
			Vector3 cameraPos; // カメラ座標(ワールド座標)
		};

	public:
		ConstBufferData* constMap = nullptr;
		Microsoft::WRL::ComPtr<ID3D12Resource> constBuffer;
		Vector3 eye = { 0, 0, -50.0f }, target, up = Vector3::MakeAxis(Axis::Y);
		Angle fovAngleY = 45; // 画角(度)
		// アスペクト比(基本はWIN_SIZEに準拠)
		float aspectRatio = WIN_SIZE.x / WIN_SIZE.y;
		float nearZ = 0.1f, farZ = 1000.0f; // 映る範囲
		// シェイク機能
		std::unique_ptr<CameraShake> shake;

		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="shakeProp">シェイク情報</param>
		void Initialize(const CameraShake::Prop* shakeProp = nullptr);
		// 更新
		void Update();
		// カメラ移動
		void CameraMove(const Vector3& move);
		// ビュー行列とプロジェクション行列を掛け合わせた行列
		Matrix4 GetViewProjectionMatrix() const { return matViewProjection; }
	};
}