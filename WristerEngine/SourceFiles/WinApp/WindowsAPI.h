#pragma once
#include <Windows.h>
#include "Vector.h"
#include <string>

namespace WristerEngine
{
	// 画質
	static const Vector2 FullHD = { 1920, 1080 };
	static const Vector2 WQHD = { 2560, 1440 };
	static const Vector2 UltraHD = { 3840, 2160 };

	// ウィンドウサイズ
	inline Vector2 WIN_SIZE = FullHD;
	// ウィンドウサイズの半分(UI描画などに使う)
	inline Vector2 HALF_WIN_SIZE = WIN_SIZE / 2.0f;

	// ウィンドウクラス処理
	class WindowsAPI final
	{
	public:
		enum class SizeChangeMode
		{
			None,        // サイズ変更不可
			Normal,      // 自由変更
			FixedAspect, // アスペクト比一定
		};

	private:
		HWND hwnd{};
		WNDCLASSEX w{};
		bool isInWindowCursor = false;
		RECT windowRect{};
		RECT inWindowCursorOffset{};
		SizeChangeMode sizeChangeMode = SizeChangeMode::Normal;
		UINT windowStyle = WS_OVERLAPPEDWINDOW;
		float aspectRatio = WIN_SIZE.x / WIN_SIZE.y;
		bool isFullscreen = false;

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
		bool ProcessMessage() const;
		// 終了
		void Finalize() const;

		// カーソルが画面外に出ないようにする
		void InWindowCursor() const;

		// カーソルの位置を設定
		void SetCursorPosition(const Vector2& pos) const { SetCursorPos((int32_t)pos.x, (int32_t)pos.y); }
		// マウスカーソルの表示 / 非表示
		void SetShowCursor(bool isShow) const { ShowCursor(isShow); }
		// カーソルが画面外に出ないようにするフラグ
		void SetIsInWindowCursor(bool isInWindowCursor_) { isInWindowCursor = isInWindowCursor_; }
		void SetInWindowCursorOffset(const RECT& inWindowCursorOffset_) { inWindowCursorOffset = inWindowCursorOffset_; }

		/// <summary>
		/// フルスクリーン設定
		/// </summary>
		/// <param name="fullscreen">フルスクリーンにするかどうか</param>
		void SetFullscreen(bool fullscreen);

		/// <summary>
		/// サイズ変更モードの設定
		/// </summary>
		/// <returns></returns>
		void SetSizeChangeMode(SizeChangeMode sizeChangeMode);

		// getter
		HWND GetHwnd() const { return hwnd; }
		HINSTANCE GetHInstance() const { return w.hInstance; }
		// マウス座標を取得する(ウィンドウ基準)
		Vector2 GetScreenCursorPos() const;
	};
}