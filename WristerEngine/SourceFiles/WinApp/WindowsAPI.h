#pragma once
#include <Windows.h>
#include "Vector.h"
#include <string>

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
		HWND hwnd;
		WNDCLASSEX w{};
		WindowsAPI() = default;

		// ウィンドウプロシージャ
		static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	public:
		WindowsAPI(const WindowsAPI& obj) = delete;

		// インスタンス取得
		static WindowsAPI* GetInstance();
		// 初期化
		void Initialize(const std::wstring& windowName = L"");
		// メッセージの処理
		bool ProcessMessage();
		// 終了
		void Finalize();

		// getter
		HWND GetHwnd() const { return hwnd; }
		HINSTANCE GetHInstance() const { return w.hInstance; }
	};
}