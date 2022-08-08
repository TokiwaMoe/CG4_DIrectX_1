#include"Input.h"
#include<Windows.h>
#include <assert.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

Input::Input(){}

Input::~Input(){}

Input* Input::GetInstance()
{
	static Input instance;

	return &instance;
}

void Input::Initialize(WinApp *winApp)
{
	HRESULT result;

	//DirectInputのインスタンス生成
	result = DirectInput8Create(
		winApp->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);
	//キーボードデバイス生成
	result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);
	//入力データ形式のセット
	result = devkeyboard->SetDataFormat(&c_dfDIKeyboard);
	//排他制御レベルのセット
	result = devkeyboard->SetCooperativeLevel(winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

	//借りてきたWinAppをインスタンスを記録
	this->winApp = winApp;
	
}

void Input::MouseInitialize(WinApp* winApp)
{
	HRESULT result;
	//マウスデバイス生成
	result = dinput->CreateDevice(GUID_SysMouse, &devMouse, NULL);
	if (devMouse != nullptr)
	{
		//入力データ形式のセット
		result = devMouse->SetDataFormat(&c_dfDIMouse);
		//排他制御レベルのセット
		result = devMouse->SetCooperativeLevel(winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	}
}

void Input::PadInitialize(WinApp* winApp)
{
	HRESULT result;
	//パッドデバイス生成
	result = dinput->CreateDevice(GUID_Joystick, &devPad, NULL);
	if (devPad != nullptr)
	{
		//入力データ形式のセット
		result = devPad->SetDataFormat(&c_dfDIJoystick);
		// 軸モードを絶対値モードとして設定
		DIPROPDWORD diprop;
		ZeroMemory(&diprop, sizeof(diprop));
		diprop.diph.dwSize = sizeof(diprop);
		diprop.diph.dwHeaderSize = sizeof(diprop.diph);
		diprop.diph.dwHow = DIPH_DEVICE;
		diprop.diph.dwObj = 0;
		diprop.dwData = DIPROPAXISMODE_ABS;

		// 軸モードを変更
		devPad->SetProperty(DIPROP_AXISMODE, &diprop.diph);

		// X軸の値の範囲設定
		DIPROPRANGE diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.diph.dwObj = DIJOFS_X;
		diprg.lMin = -1000;
		diprg.lMax = 1000;

		// X軸の値の範囲設定
		devPad->SetProperty(DIPROP_RANGE, &diprg.diph);

		// Y軸の値の範囲設定
		diprg.diph.dwObj = DIJOFS_Y;
		devPad->SetProperty(DIPROP_RANGE, &diprg.diph);
		//排他制御レベルのセット
		result = devPad->SetCooperativeLevel(winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	}
	
}

void Input::MouseUpdate()
{
	HRESULT result;

	if (devMouse != nullptr)
	{
		//マウスの情報取得開始
		result = devMouse->Acquire();
		//前回のキー入力を保存
		oldMouse = mouse;
		//マウスの入力情報を取得
		result = devMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouse);
	}
	
}

void Input::PadUpdate()
{
	HRESULT result;

	if (devPad != nullptr)
	{
		//パッドの情報取得開始
		result = devPad->Acquire();
		//前回のキー入力を保存
		oldPad = pad;
		//パッドの入力情報を取得
		result = devPad->GetDeviceState(sizeof(DIJOYSTATE), &pad);
		//リセット
		for (int i = 0; i < 32; i++)
		{
			isPush[i] = false;
		}
	}
	
}

void Input::KeyUpdate()
{
	HRESULT result;

	//キーボード情報の取得開始
	result = devkeyboard->Acquire();
	//前回のキー入力を保存
	memcpy(oldkey, key, sizeof(key));
	//全キーの入力情報を取得する
	result = devkeyboard->GetDeviceState(sizeof(key), key);
}

bool Input::PushKey(BYTE keyNumber)
{
	if (key[keyNumber])
	{
		return true;
	}
	return false;
}

bool Input::PushMouse(int MouseNumber)
{
	if (mouse.rgbButtons[MouseNumber])
	{
		return true;
	}
	return false;
}

bool Input::TriggerKey(BYTE keyNumber)
{
	if (key[keyNumber] && !oldkey[keyNumber])
	{
		return true;
	}
	return false;
}

bool Input::TriggerMouse(int MouseNumber)
{
	if (mouse.rgbButtons[MouseNumber] && !oldMouse.rgbButtons[MouseNumber])
	{
		return true;
	}
	return false;
}

Input::MouseMove Input::GetMouseMove()
{
	MouseMove tmp;
	tmp.lX = mouse.lX;
	tmp.lY = mouse.lY;
	tmp.lZ = mouse.lZ;
	return tmp;
}

bool Input::StickTilt(int stick)
{
	LONG unresponsive_range = 900;

	assert(0 <= stick && stick < 4);

	if (pad.lX < -unresponsive_range && stick == Stick_Left)
	{
		return true;
	}
	else if (pad.lX > unresponsive_range && stick == Stick_Right)
	{
		return true;
	}
	if (pad.lY < -unresponsive_range && stick == Stick_Up)
	{
		return true;
	}
	else if (pad.lY > unresponsive_range && stick == Stick_Down)
	{
		return true;
	}

	DWORD length = 1000; // 原点から最小、最大までの長さ
	float y_vec = (pad.lY - unresponsive_range) / (length - unresponsive_range);

	return false;
}
