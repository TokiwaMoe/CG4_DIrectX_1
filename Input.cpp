#include"Input.h"
#include<Windows.h>
#include<wrl.h>
#define DIRECTINPUT_VERSION		0x0800	//DirectInputのバージョン指定
#include<dinput.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include"WinApp.h"
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

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
	//入力データ形式のセット
	result = devMouse->SetDataFormat(&c_dfDIMouse);
	//排他制御レベルのセット
	result = devMouse->SetCooperativeLevel(winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
}

void Input::MouseUpdate()
{
	HRESULT result;

	//前回のキー入力を保存
	oldMouse = mouse;
	//マウスの情報取得開始
	result = devMouse->Acquire();
	//マウスの入力情報を取得
	result = devMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouse);
}

void Input::KeyUpdate()
{
	HRESULT result;

	//前回のキー入力を保存
	memcpy(oldkey, key, sizeof(key));

	//キーボード情報の取得開始
	result = devkeyboard->Acquire();
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
