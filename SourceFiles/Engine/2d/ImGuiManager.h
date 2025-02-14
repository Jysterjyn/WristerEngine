#pragma once
#include <string>
#include <d3dx12.h>
#include <wrl.h>
#include "Quaternion.h"
#include "Color.h"

namespace WristerEngine::_2D
{
	// ImGUIの管理
	class ImGuiManager final
	{
	private:
		ImGuiManager() = delete;
		~ImGuiManager() = delete;
		static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap;

	public:
		// 初期化
		static void Initialize();
		// ImGui受付開始
		static void Begin();
		// ImGui受付終了
		static void End();
		// 画面への描画
		static void Draw();
		// 終了処理
		static void Finalize();
		// ベクトル値の描画
		static void PrintVector(std::string str, const Vector2& vec);
		static void PrintVector(std::string str, const Vector3& vec);
		// クォータニオンの描画
		static void PrintQuaternion(std::string str, const Quaternion& q);
		// ベクトル値のスライダー変更
		static void SliderVector(std::string str, Vector2& vec, float minVal, float maxVal);
		static void SliderVector(std::string str, Vector3& vec, float minVal, float maxVal);
		// ベクトル値のドラッグ変更
		static void DragVector(std::string str, Vector2& vec, float spd = 0.1f);
		static void DragVector(std::string str, Vector3& vec, float spd = 0.1f);
		// ベクトル値の入力変更
		static void InputVector(std::string str, Vector2& vec);
		static void InputVector(std::string str, Vector3& vec);
		// 色変更
		static void ColorEdit(std::string str, ColorRGB& color);
		static void ColorEdit(std::string str, ColorRGBA& color);
	};
}