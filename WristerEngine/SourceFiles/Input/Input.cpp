#include "Input.h"
#include "D3D12Common.h"
#include <cmath>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
using namespace WristerEngine;

Microsoft::WRL::ComPtr<IDirectInput8> Input::directInput;

//ゲームパッドデバイスの作成-デバイス列挙の結果を受け取る構造体
struct DeviceEnumParameter
{
	std::vector<Input::Joystick> joysticks;
};

Input* Input::GetInstance()
{
	static Input input;
	return &input;
}

std::vector<Input::Joystick> Input::Joystick::Create()
{
	DeviceEnumParameter parameter{};

	// GAMEPADを調べる
	directInput->EnumDevices(
		DI8DEVTYPE_GAMEPAD,			// 検索するデバイスの種類
		DeviceFindCallBack,			// 発見時に実行する関数
		&parameter,					// 関数に渡す値
		DIEDFL_ATTACHEDONLY			// 検索方法
	);

	// JOYSTICKを調べる
	directInput->EnumDevices(
		DI8DEVTYPE_JOYSTICK,
		DeviceFindCallBack,
		&parameter,
		DIEDFL_ATTACHEDONLY
	);

	std::vector<Input::Joystick> joysticks = parameter.joysticks;

	// どちらも見つけることが出来なかったら失敗
	if (parameter.joysticks.empty()) { return parameter.joysticks; }

	for(auto& j:parameter.joysticks)
	{
		// デバイスが生成されてない
		if (!j.device) { continue; }

		// 制御開始
		DIDEVCAPS cap;
		j.device->GetCapabilities(&cap);
		// ポーリング判定
		if (cap.dwFlags & DIDC_POLLEDDATAFORMAT)
		{
			// ポーリング開始
			j.device->Acquire();
			j.device->Poll();
		}
	}

	return parameter.joysticks;
}

void Input::Initialize()
{
	Result result;
	WindowsAPI* wAPI = WindowsAPI::GetInstance();

	result = DirectInput8Create(wAPI->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	// キーボード
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);
	result = keyboard->SetCooperativeLevel(wAPI->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	// マウス
	result = directInput->CreateDevice(GUID_SysMouse, &mouse, NULL);
	result = mouse->SetDataFormat(&c_dfDIMouse2);
	result = mouse->SetCooperativeLevel(wAPI->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

	// ゲームパッド
	joysticks = Joystick::Create();
}

bool SetUpGamePadProperty(LPDIRECTINPUTDEVICE8 device)
{
	// 軸モードを絶対値モードとして設定
	DIPROPDWORD diprop;
	ZeroMemory(&diprop, sizeof(diprop));
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.diph.dwObj = 0;
	diprop.dwData = DIPROPAXISMODE_ABS;
	if (FAILED(device->SetProperty(DIPROP_AXISMODE, &diprop.diph))) { return false; }

	// X軸の値の範囲設定
	DIPROPRANGE diprg;
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.diph.dwObj = DIJOFS_X;
	diprg.lMin = -Input::PADSTICK_MAX_VAL;
	diprg.lMax = Input::PADSTICK_MAX_VAL;
	if (FAILED(device->SetProperty(DIPROP_RANGE, &diprg.diph))) { return false; }

	// Y軸の値の範囲設定
	diprg.diph.dwObj = DIJOFS_Y;
	if (FAILED(device->SetProperty(DIPROP_RANGE, &diprg.diph))) { return false; }

	// RX軸の値の範囲設定
	diprg.diph.dwObj = DIJOFS_RX;
	if (FAILED(device->SetProperty(DIPROP_RANGE, &diprg.diph))) { return false; }
	// RY軸の値の範囲設定
	diprg.diph.dwObj = DIJOFS_RY;
	if (FAILED(device->SetProperty(DIPROP_RANGE, &diprg.diph))) { return false; }

	return true;
}

int CALLBACK Input::DeviceFindCallBack(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
	DeviceEnumParameter* parameter = (DeviceEnumParameter*)pvRef;
	IDirectInputDevice8* device = nullptr;

	Joystick joysticks;

	// デバイス生成
	HRESULT hr = directInput->CreateDevice(
		lpddi->guidInstance,
		&joysticks.device,
		NULL);

	if (FAILED(hr)) { return DIENUM_STOP; }

	// 入力フォーマットの指定
	device = joysticks.device.Get();
	hr = device->SetDataFormat(&c_dfDIJoystick);

	if (FAILED(hr)) { return DIENUM_STOP; }

	// プロパティの設定
	if (!SetUpGamePadProperty(device)) { return DIENUM_STOP; }

	// 協調レベルの設定
	WindowsAPI* wAPI = WindowsAPI::GetInstance();
	device->SetCooperativeLevel(wAPI->GetHwnd(), DISCL_EXCLUSIVE | DISCL_FOREGROUND);

	parameter->joysticks.push_back(joysticks);

	return DIENUM_CONTINUE;
}

void Input::StartGamePadControl()
{

}

void Input::Joystick::Update()
{
	device->Acquire();
	statePre = state;
	device->GetDeviceState(sizeof(state), &state);
}

void Input::Update()
{
	keyboard->Acquire();
	oldkey = key;
	keyboard->GetDeviceState((DWORD)key.size(), (LPVOID)key.data());

	mouse->Acquire();
	mouseStatePre = mouseState;
	mouse->GetDeviceState(sizeof(mouseState), &mouseState);

	for (auto& j : joysticks) { j.Update(); }
}

bool WristerEngine::Input::IsTrigger(int32_t stickNo, JoyPad button) const
{
	Joystick stick = joysticks[stickNo];
	return !stick.statePre.rgbButtons[(int)button] && stick.state.rgbButtons[(int)button];
}

bool Input::IsAnyInput(std::vector<Key>& keys) const
{
	for (auto& key_ : keys) { if (IsInput(key_)) { return true; } }
	return false;
}

Input::PadState Input::GetPadState(int32_t stickNo) const
{
	DIJOYSTATE joyState = GetJoyState(stickNo);
	float angle = joyState.rgdwPOV[0] * PI / 18000.0f;
	Vector2 dirKey;
	if (joyState.rgdwPOV[0] != -1) { dirKey = { std::sin(angle), std::cos(angle) }; }
	return PadState(joyState.lX, joyState.lY, joyState.lRx, joyState.lRy, joyState.lZ, dirKey);
}

void Input::SetJoystickDeadZone(int32_t stickNo, int32_t deadZoneL, int32_t deadZoneR)
{
	joysticks[stickNo].deadZoneL = deadZoneL;
	joysticks[stickNo].deadZoneR = deadZoneR;
}

Vector2 Input::PadState::LNormalize(int32_t stickNo) const
{
	auto padState = Input::GetInstance()->GetPadState(stickNo);
	Vector2 padStickVec = { (float)padState.lX, (float)padState.lY };
	padStickVec /= (float)Input::PADSTICK_MAX_VAL;
	return padStickVec;
}

Vector2 Input::PadState::RNormalize(int32_t stickNo) const
{
	auto padState = Input::GetInstance()->GetPadState(stickNo);
	Vector2 padStickVec = { (float)padState.rX, (float)padState.rY };
	padStickVec /= (float)Input::PADSTICK_MAX_VAL;
	return padStickVec;
}

Vector2 Input::ConLStick(int32_t stickNo, const float spd) const
{
	Vector2 vec;
	// X軸について
	if (std::abs(GetPadState(stickNo).lX) > joysticks[stickNo].deadZoneL) { vec.x = (float)GetPadState(stickNo).lX; }
	else { vec.x = 0.0f; }
	// Y軸について
	if (std::abs(GetPadState(stickNo).lY) > joysticks[stickNo].deadZoneL) { vec.y = -(float)GetPadState(stickNo).lY; }
	else { vec.y = 0.0f; }

	return Normalize(vec) * spd;
}

Vector2 Input::ConRStick(int32_t stickNo, const float spd) const
{
	Vector2 vec;
	// X軸について
	if (std::abs(GetPadState(stickNo).rX) > joysticks[stickNo].deadZoneR) { vec.x = (float)GetPadState(stickNo).rX; }
	else { vec.x = 0.0f; }
	// Y軸について
	if (std::abs(GetPadState(stickNo).rY) > joysticks[stickNo].deadZoneR) { vec.y = -(float)GetPadState(stickNo).rY; }
	else { vec.y = 0.0f; }

	return Normalize(vec) * spd;
}