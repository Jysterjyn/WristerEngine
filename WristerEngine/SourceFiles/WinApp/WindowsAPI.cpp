#include "WindowsAPI.h"
#include <imgui_impl_win32.h>
#pragma comment(lib,"winmm.lib")

using namespace WristerEngine;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// ウィンドウプロシージャ
LRESULT WindowsAPI::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) { return true; }

	// メッセージに応じてゲーム固有の処理を行う
	switch (msg)
	{
		// ウィンドウが破棄された
	case WM_DESTROY:
		// OSに対して、アプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	// 標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

WindowsAPI* WindowsAPI::GetInstance()
{
	static WindowsAPI wAPI;
	return &wAPI;
}

bool WindowsAPI::ProcessMessage()
{
	MSG msg{}; // メッセージ

	// メッセージがある?
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg); // キー入力メッセージの処理
		DispatchMessage(&msg); // プロシージャにメッセージを送る
	}

	return msg.message == WM_QUIT;
}

void WindowsAPI::Initialize(const std::wstring& windowName)
{
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc; // ウィンドウプロシージャを設定
	w.lpszClassName = L"DirectXGame"; // ウィンドウクラス名
	w.hInstance = GetModuleHandle(nullptr); // ウィンドウハンドル
	w.hCursor = LoadCursor(NULL, IDC_ARROW); // カーソル指定

	// ウィンドウクラスをOSに登録する
	RegisterClassEx(&w);

	RECT wrc = { 0, 0, (int)WIN_SIZE.x, (int)WIN_SIZE.y };
	// 自動でサイズを補正する
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	std::wstring windowName_ = windowName;

	hwnd = CreateWindow(w.lpszClassName, // クラス名
		windowName.c_str(), // タイトルバーの文字
		WS_OVERLAPPEDWINDOW, // 標準的なウィンドウスタイル
		CW_USEDEFAULT, // 表示X座標(OSに任せる)
		CW_USEDEFAULT, // 表示X座標(OSに任せる)
		wrc.right - wrc.left, // ウィンドウ横幅
		wrc.bottom - wrc.top, // ウィンドウ縦幅
		nullptr, // 親ウィンドウハンドル
		nullptr, // メニューハンドル
		w.hInstance, // 呼び出しアプリケーションハンドル
		nullptr); // オプション

	// ウィンドウを表示状態にする
	ShowWindow(hwnd, SW_SHOW);

	// システムタイマーの分解能を上げる
	timeBeginPeriod(1);
}

void WindowsAPI::Finalize() const
{
	// ウィンドウクラスを登録解除
	UnregisterClass(w.lpszClassName, w.hInstance);
}

Vector2 WindowsAPI::GetScreenCursorPos() const
{
	POINT mousePosition{};
	// マウス座標(スクリーン座標)を取得する
	GetCursorPos(&mousePosition);
	// クライアントエリア座標に変換する
	ScreenToClient(hwnd, &mousePosition);
	return Vector2((float)mousePosition.x, (float)mousePosition.y);
}