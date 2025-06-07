#pragma once
#include <Windows.h>
#include "Vector.h"
#include <string>
#include <WristerEngineUtility.h>

namespace WristerEngine
{
	// ウィンドウサイズ
	const Vector2 WIN_SIZE = { 1280,720 };
	// ウィンドウサイズの半分(UI描画などに使う)
	const Vector2 HALF_WIN_SIZE = WIN_SIZE / 2.0f;
	
	// ウィンドウクラス処理
	class WindowsAPI final
	{
	private:
		HWND hwnd{};
		WNDCLASSEX w{};

		WindowsAPI() = default;
		~WindowsAPI() = default;
		WindowsAPI(const WindowsAPI&) = delete;
		WindowsAPI& operator=(const WindowsAPI&) = delete;

		// ウィンドウプロシージャ
		static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	public:
		// インスタンス取得
		static WindowsAPI* GetInstance();
		// 初期化
		void Initialize(const std::wstring& windowName = L"");
		// メッセージの処理
		bool ProcessMessage();
		// 終了
		void Finalize() const;
		
		// カーソルの位置を設定
		void SetCursorPosition(Vector2 pos) { SetCursorPos((int32_t)pos.x, (int32_t)pos.y); }

		// getter
		HWND GetHwnd() const { return hwnd; }
		HINSTANCE GetHInstance() const { return w.hInstance; }
		// マウス座標を取得する
		Vector2 GetScreenCursorPos() const;
	};
}