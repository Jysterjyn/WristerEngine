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
		bool isInWindowCursor = false;
		RECT inWindowCursorOffset{};

		WindowsAPI() = default;
		~WindowsAPI() = default;
		WindowsAPI(const WindowsAPI&) = delete;
		WindowsAPI& operator=(const WindowsAPI&) = delete;

		// ウィンドウプロシージャ
		static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

		// カーソルが画面外に出ないようにする
		void InWindowCursor() const;

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
		void SetCursorPosition(const Vector2& pos) const { SetCursorPos((int32_t)pos.x, (int32_t)pos.y); }
		// マウスカーソルの表示 / 非表示
		void SetShowCursor(bool isShow) const { ShowCursor(isShow); }
		// カーソルが画面外に出ないようにするフラグ
		void SetIsInWindowCursor(bool isInWindowCursor_) { isInWindowCursor = isInWindowCursor_; }
		void InWindowCursorOffset(const RECT& inWindowCursorOffset_) { inWindowCursorOffset = inWindowCursorOffset_; }

		// getter
		HWND GetHwnd() const { return hwnd; }
		HINSTANCE GetHInstance() const { return w.hInstance; }
		// マウス座標を取得する(ウィンドウ基準)
		Vector2 GetScreenCursorPos() const;
	};
}