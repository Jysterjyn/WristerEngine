#pragma once
#include "FbxModel.h"
#include "Transform.h"
#include "LightGroup.h"

namespace WristerEngine::_3D
{
	class FbxObject3d
	{
	protected:
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	public:
		static const int MAX_BONES = 32;

		// 定数バッファ用データ構造体（座標変換行列用）
		struct ConstBufferData
		{
			Matrix4 viewproj;    // ビュープロジェクション行列
			Matrix4 world; // ワールド行列
			Vector3 cameraPos; // カメラ座標（ワールド座標）
		};

		struct ConstBufferDataSkin
		{
			Matrix4 bones[MAX_BONES];
		};

	private: // 静的メンバ変数
		static LightGroup* lightGroup;

	protected: // メンバ変数
		// 定数バッファ
		ComPtr<ID3D12Resource> constBuff, constBuffSkin;
		ConstBufferData* constMap = nullptr;
		ConstBufferDataSkin* constMapSkin = nullptr;
		Transform* transform;
		// モデル
		FbxModel* model = nullptr;
		FbxTime frameTime, startTime, endTime, currentTime;
		bool isPlay = false;

	public:
		static void SetLightGroup(LightGroup* lightGroup_) { lightGroup = lightGroup_; }

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize(Transform* transform, FbxModel* model);

		/// <summary>
		/// 毎フレーム処理
		/// </summary>
		void Update();

		/// <summary>
		/// 描画
		/// </summary>
		void Draw();

		void PlayAnimation();
	};
}