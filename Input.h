#pragma once
#include<wrl.h>
#define DIRECTINPUT_VERSION		0x0800	//DirectInputのバージョン指定
#include<dinput.h>
#include"WinApp.h"

class Input final
{
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
private:
	//コンストラクタ
	Input();
	//デストラクタ
	~Input();
public:
	//コピーコンストラクタを無効
	Input(const Input& obj) = delete;
	//代入演算子を無効
	Input& operator=(const Input& obj) = delete;
	//インスタンスにアクセス
	static Input* GetInstance();
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
	
};