#pragma once
#include "Light.h"
#include "CircleShadow.h"
#include <wrl.h>
#include <d3d12.h>
#include <memory>
#include <array>

namespace WristerEngine
{
	// ライト
	class LightGroup
	{
	public:
		static const int DIR_LIGHT_NUM = 3;
		static const int POINT_LIGHT_NUM = 3;
		static const int SPOT_LIGHT_NUM = 3;
		static const int CIRCLE_SHADOW_NUM = 3;

	private:
		template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

		struct ConstBufferData
		{
			ColorRGB ambientColor;
			float pad1;
			DirectionalLight::ConstBufferData dirLights[DIR_LIGHT_NUM];
			PointLight::ConstBufferData pointLights[POINT_LIGHT_NUM];
			SpotLight::ConstBufferData spotLights[SPOT_LIGHT_NUM];
			CircleShadow::ConstBufferData circleShadows[CIRCLE_SHADOW_NUM];
		};

		ComPtr<ID3D12Resource> constBuff;
		ColorRGB ambientColor;
		std::array<DirectionalLight, DIR_LIGHT_NUM> dirLights;
		std::array<PointLight, POINT_LIGHT_NUM> pointLights;
		std::array<SpotLight, SPOT_LIGHT_NUM> spotLights;
		std::array<CircleShadow, CIRCLE_SHADOW_NUM> circleShadows;
		ConstBufferData* constMap = nullptr;

		// 定数バッファ転送
		void TransferConstBuffer();
		// 初期化
		void Initialize();
		// 標準のライト設定
		void DefaultLightSetting();

	public:
		/// <summary>
		/// インスタンス生成
		/// </summary>
		/// <returns>インスタンス</returns>
		static std::unique_ptr<LightGroup> Create();
		// 更新
		void Update(){ TransferConstBuffer(); }
		// 描画
		void Draw(UINT rootParameterIndex);
		// 環境光を変更
		void SetAmbientColor(const ColorRGB& color) { ambientColor = color; }
		// 平行光源を取得
		DirectionalLight* GetDirectionalLight(uint32_t index);
		// 点光源を取得
		PointLight* GetPointLight(uint32_t index);
		// スポットライトを取得
		SpotLight* GetSpotLight(uint32_t index);
		// 丸影を取得
		CircleShadow* GetCircleShadow(uint32_t index);
	};
}