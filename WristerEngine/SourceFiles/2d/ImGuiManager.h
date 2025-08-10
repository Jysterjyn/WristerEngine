#pragma once
#include <string>
#include "Quaternion.h"
#include "Color.h"

namespace WristerEngine
{
	// ImGUIの管理
	class ImGuiManager final
	{
		ImGuiManager() = default;
		~ImGuiManager() = default;
		ImGuiManager(const ImGuiManager&) = delete;
		ImGuiManager& operator=(const ImGuiManager&) = delete;

	public:
		static ImGuiManager* GetInstance();
		// 初期化
		void Initialize();
		// ImGui受付開始
		void Begin();
		// ImGui受付終了
		void End();
		// 画面への描画
		void Draw();
		// 終了処理
		void Finalize();
		// ベクトル値の描画
		void PrintVector(std::string str, const Vector2& vec);
		void PrintVector(std::string str, const Vector3& vec);
		// クォータニオンの描画
		void PrintQuaternion(std::string str, const Quaternion& q);
		// ベクトル値のスライダー変更
		void SliderVector(std::string str, Vector2& vec, float minVal, float maxVal);
		void SliderVector(std::string str, Vector3& vec, float minVal, float maxVal);
		// ベクトル値のドラッグ変更
		void DragVector(std::string str, Vector2& vec, float spd = 0.1f);
		void DragVector(std::string str, Vector3& vec, float spd = 0.1f);
		// ベクトル値の入力変更
		void InputVector(std::string str, Vector2& vec);
		void InputVector(std::string str, Vector3& vec);
		// 色変更
		void ColorEdit(std::string str, ColorRGB& color);
		void ColorEdit(std::string str, ColorRGBA& color);
	};
}