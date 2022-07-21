#pragma once
#include<wrl.h>
#define DIRECTINPUT_VERSION		0x0800	//DirectInputのバージョン指定
#include<dinput.h>
#include"WinApp.h"
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>

class Input 
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	//マウス
	enum Mouse
	{
		M_Left, M_Right, M_Middle
	};
	struct MouseMove {
		LONG    lX;
		LONG    lY;
		LONG    lZ;
	};
public:
	//namespace省略
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
private:
	//WindowAPI
	WinApp* winApp = nullptr;
public://メンバ関数
	//初期化
	void Initialize(WinApp *winApp);
	void MouseInitialize(WinApp* winApp);
	//更新
	void KeyUpdate();
	void MouseUpdate();

/// <summary>
/// キーの押下をチェック
/// </summary>
/// <parm name="keyNumber">キー番号(DIK_0 等)</param>
/// <return>押されているか</returns>
	bool PushKey(BYTE keyNumber);
	bool PushMouse(int MouseNumber);

/// <summary>
/// キーのトリガーをチェック
/// </summary>
/// <parm name="keyNumber">キー番号(DIK_0 等)</param>
/// <return>押されているか</returns>
	bool TriggerKey(BYTE keyNumber);
	bool TriggerMouse(int MouseNumber);

	/// <summary>
	/// マウス移動量を取得
	/// </summary>
	/// <returns>マウス移動量</returns>
	MouseMove GetMouseMove();

	XMFLOAT2 GetMousePos() { return mousePos; }

private: //メンバ変数
	//DirectInputのインスタンス
	ComPtr<IDirectInput8> dinput;
	//キーボードのデバイス
	ComPtr<IDirectInputDevice8> devkeyboard;
	//マウスのデバイス
	ComPtr<IDirectInputDevice8> devMouse;
	//全キー状態
	BYTE key[256] = {};
	//前回のキー
	BYTE oldkey[256] = {};
	//マウスの状態
	DIMOUSESTATE mouse = { 0 };
	//前回のマウス
	DIMOUSESTATE oldMouse = { 0 };

public:
	XMFLOAT2 mousePos = { 0,0 };
	
};