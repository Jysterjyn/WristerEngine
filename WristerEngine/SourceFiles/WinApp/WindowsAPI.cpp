#include "WindowsAPI.h"
#include <imgui_impl_win32.h>
#include <algorithm>
#pragma comment(lib,"winmm.lib")
#include <cassert>

using namespace WE;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static BOOL UnadjustWindowRectEx(LPRECT prc, DWORD dwStyle, BOOL fMenu, DWORD dwExStyle)
{
	RECT rc;
	SetRectEmpty(&rc);
	BOOL fRc = AdjustWindowRectEx(&rc, dwStyle, fMenu, dwExStyle);
	if (fRc)
	{
		prc->left -= rc.left;
		prc->top -= rc.top;
		prc->right -= rc.right;
		prc->bottom -= rc.bottom;
	}
	return fRc;
}

// ウィンドウプロシージャ
LRESULT WindowsAPI::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) { return true; }

	WindowsAPI* app = reinterpret_cast<WindowsAPI*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	// メッセージに応じてゲーム固有の処理を行う
	switch (msg)
	{
	case WM_DESTROY:        // ウィンドウが破棄された
		PostQuitMessage(0); // OSに対して、アプリの終了を伝える
		return 0;

	case WM_SIZING:
		// アスペクト比を変えるサイズ変更を許可しない
		if (app && app->sizeChangeMode == WindowsAPI::SizeChangeMode::FixedAspect)
		{
			float aspectRatio = app->aspectRatio;
			float aspectRatioRecp = 1.0f / aspectRatio;
			RECT* rect = reinterpret_cast<RECT*>(lparam);
			UnadjustWindowRectEx(
				rect, GetWindowLong(hwnd, GWL_STYLE), GetMenu(hwnd) != 0,
				GetWindowLong(hwnd, GWL_EXSTYLE));

			switch (wparam)
			{
			case WMSZ_LEFT:
			case WMSZ_BOTTOMLEFT:
			case WMSZ_RIGHT:
			case WMSZ_BOTTOMRIGHT:
				rect->bottom = rect->top + LONG((rect->right - rect->left) * aspectRatioRecp);
				break;
			case WMSZ_TOP:
			case WMSZ_TOPRIGHT:
			case WMSZ_BOTTOM:
				rect->right = rect->left + LONG((rect->bottom - rect->top) * aspectRatio);
				break;
			case WMSZ_TOPLEFT:
				rect->top = rect->bottom - LONG((rect->right - rect->left) * aspectRatioRecp);
				rect->left = rect->right - LONG((rect->bottom - rect->top) * aspectRatio);
				break;
			}

			AdjustWindowRectEx(
				rect, GetWindowLong(hwnd, GWL_STYLE), GetMenu(hwnd) != 0,
				GetWindowLong(hwnd, GWL_EXSTYLE));
		}
		break;
	}
	// 標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

WindowsAPI* WindowsAPI::GetInstance()
{
	static WindowsAPI wAPI;
	return &wAPI;
}

void WindowsAPI::Initialize(const std::wstring& windowName)
{
	// COM初期化
	HRESULT result;
	result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	assert(SUCCEEDED(result));

	// ウィンドウクラスの設定
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

	SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	// ウィンドウを表示状態にする
	ShowWindow(hwnd, SW_SHOW);

	// システムタイマーの分解能を上げる
	timeBeginPeriod(1);
}

bool WindowsAPI::ProcessMessage() const
{
	if (isInWindowCursor) { InWindowCursor(); }

	MSG msg{}; // メッセージ

	// メッセージがある?
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg); // キー入力メッセージの処理
		DispatchMessage(&msg); // プロシージャにメッセージを送る
	}

	return msg.message == WM_QUIT;
}

void WindowsAPI::Finalize() const
{
	// ウィンドウクラスを登録解除
	UnregisterClass(w.lpszClassName, w.hInstance);

	// COM終了
	CoUninitialize();
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

void WindowsAPI::InWindowCursor() const
{
	// ウィンドウ四隅の座標を取得
	RECT lect{};
	GetWindowRect(hwnd, &lect);

	// オフセットを加算
	lect.left += inWindowCursorOffset.left;
	lect.right += inWindowCursorOffset.right;
	lect.top += inWindowCursorOffset.top;
	lect.bottom += inWindowCursorOffset.bottom;

	// カーソルが画面外に出たら画面内に戻す
	POINT point{};
	GetCursorPos(&point);
	point.x = std::clamp(point.x, lect.left, lect.right);
	point.y = std::clamp(point.y, lect.top, lect.bottom);
	SetCursorPos(point.x, point.y);
}

void WindowsAPI::SetFullscreen(bool fullscreen)
{
	if (isFullscreen != fullscreen)
	{
		if (fullscreen)
		{
			// 元の状態を覚えておく
			GetWindowRect(hwnd, &windowRect);

			// 仮想フルスクリーン化
			SetWindowLong(hwnd, GWL_STYLE,
				windowStyle &
				~(WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_THICKFRAME));

			RECT fullscreenRect{ 0 };
			HMONITOR monitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
			MONITORINFO info{};
			info.cbSize = sizeof(info);
			GetMonitorInfo(monitor, &info);
			fullscreenRect.right = info.rcMonitor.right - info.rcMonitor.left;
			fullscreenRect.bottom = info.rcMonitor.bottom - info.rcMonitor.top;

			SetWindowPos(
				hwnd, HWND_NOTOPMOST, fullscreenRect.left, fullscreenRect.top, fullscreenRect.right,
				fullscreenRect.bottom, SWP_FRAMECHANGED | SWP_NOACTIVATE);
			ShowWindow(hwnd, SW_MAXIMIZE);
		}
		else
		{
			// 通常ウィンドウに戻す
			SetWindowLong(hwnd, GWL_STYLE, windowStyle);

			SetWindowPos(
				hwnd, HWND_NOTOPMOST, windowRect.left, windowRect.top,
				windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
				SWP_FRAMECHANGED | SWP_NOACTIVATE);

			ShowWindow(hwnd, SW_NORMAL);
		}
	}

	isFullscreen = fullscreen;
}

void WindowsAPI::SetSizeChangeMode(SizeChangeMode sizeChangeMode_)
{
	sizeChangeMode = sizeChangeMode_;
	if (sizeChangeMode_ == SizeChangeMode::None)
	{
		windowStyle &= ~WS_THICKFRAME;
	}
	else
	{
		// アスペクト比変更不可なので現在のアスペクト比を持っておく
		if (sizeChangeMode_ == SizeChangeMode::FixedAspect)
		{
			RECT clientRect{};
			GetClientRect(hwnd, &clientRect);
			aspectRatio = float(clientRect.right - clientRect.left) /
				float(clientRect.bottom - clientRect.top);
		}
		windowStyle |= WS_THICKFRAME;
	}
	SetWindowLong(hwnd, GWL_STYLE, windowStyle);
	SetWindowPos(
		hwnd, NULL, 0, 0, 0, 0, (SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED));
	ShowWindow(hwnd, SW_NORMAL);
}
