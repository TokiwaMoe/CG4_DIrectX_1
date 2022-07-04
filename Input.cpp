#include"Input.h"
#include<Windows.h>

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

	//DirectInput�̃C���X�^���X����
	result = DirectInput8Create(
		winApp->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);
	//�L�[�{�[�h�f�o�C�X����
	result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);
	//���̓f�[�^�`���̃Z�b�g
	result = devkeyboard->SetDataFormat(&c_dfDIKeyboard);
	//�r�����䃌�x���̃Z�b�g
	result = devkeyboard->SetCooperativeLevel(winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

	//�؂�Ă���WinApp���C���X�^���X���L�^
	this->winApp = winApp;
	
}

void Input::MouseInitialize(WinApp* winApp)
{
	HRESULT result;
	//�}�E�X�f�o�C�X����
	result = dinput->CreateDevice(GUID_SysMouse, &devMouse, NULL);
	//���̓f�[�^�`���̃Z�b�g
	result = devMouse->SetDataFormat(&c_dfDIMouse);
	//�r�����䃌�x���̃Z�b�g
	result = devMouse->SetCooperativeLevel(winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
}

void Input::MouseUpdate()
{
	HRESULT result;

	//�O��̃L�[���͂�ۑ�
	oldMouse = mouse;
	//�}�E�X�̏��擾�J�n
	result = devMouse->Acquire();
	//�}�E�X�̓��͏����擾
	result = devMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouse);
}

void Input::KeyUpdate()
{
	HRESULT result;

	//�O��̃L�[���͂�ۑ�
	memcpy(oldkey, key, sizeof(key));

	//�L�[�{�[�h���̎擾�J�n
	result = devkeyboard->Acquire();
	//�S�L�[�̓��͏����擾����
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
