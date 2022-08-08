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
	if (devMouse != nullptr)
	{
		//���̓f�[�^�`���̃Z�b�g
		result = devMouse->SetDataFormat(&c_dfDIMouse);
		//�r�����䃌�x���̃Z�b�g
		result = devMouse->SetCooperativeLevel(winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	}
}

void Input::PadInitialize(WinApp* winApp)
{
	HRESULT result;
	//�p�b�h�f�o�C�X����
	result = dinput->CreateDevice(GUID_Joystick, &devPad, NULL);
	if (devPad != nullptr)
	{
		//���̓f�[�^�`���̃Z�b�g
		result = devPad->SetDataFormat(&c_dfDIJoystick);
		// �����[�h���Βl���[�h�Ƃ��Đݒ�
		DIPROPDWORD diprop;
		ZeroMemory(&diprop, sizeof(diprop));
		diprop.diph.dwSize = sizeof(diprop);
		diprop.diph.dwHeaderSize = sizeof(diprop.diph);
		diprop.diph.dwHow = DIPH_DEVICE;
		diprop.diph.dwObj = 0;
		diprop.dwData = DIPROPAXISMODE_ABS;

		// �����[�h��ύX
		devPad->SetProperty(DIPROP_AXISMODE, &diprop.diph);

		// X���̒l�͈̔͐ݒ�
		DIPROPRANGE diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.diph.dwObj = DIJOFS_X;
		diprg.lMin = -1000;
		diprg.lMax = 1000;

		// X���̒l�͈̔͐ݒ�
		devPad->SetProperty(DIPROP_RANGE, &diprg.diph);

		// Y���̒l�͈̔͐ݒ�
		diprg.diph.dwObj = DIJOFS_Y;
		devPad->SetProperty(DIPROP_RANGE, &diprg.diph);
		//�r�����䃌�x���̃Z�b�g
		result = devPad->SetCooperativeLevel(winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	}
	
}

void Input::MouseUpdate()
{
	HRESULT result;

	if (devMouse != nullptr)
	{
		//�}�E�X�̏��擾�J�n
		result = devMouse->Acquire();
		//�O��̃L�[���͂�ۑ�
		oldMouse = mouse;
		//�}�E�X�̓��͏����擾
		result = devMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouse);
	}
	
}

void Input::PadUpdate()
{
	HRESULT result;

	if (devPad != nullptr)
	{
		//�p�b�h�̏��擾�J�n
		result = devPad->Acquire();
		//�O��̃L�[���͂�ۑ�
		oldPad = pad;
		//�p�b�h�̓��͏����擾
		result = devPad->GetDeviceState(sizeof(DIJOYSTATE), &pad);
		//���Z�b�g
		for (int i = 0; i < 32; i++)
		{
			isPush[i] = false;
		}
	}
	
}

void Input::KeyUpdate()
{
	HRESULT result;

	//�L�[�{�[�h���̎擾�J�n
	result = devkeyboard->Acquire();
	//�O��̃L�[���͂�ۑ�
	memcpy(oldkey, key, sizeof(key));
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

	DWORD length = 1000; // ���_����ŏ��A�ő�܂ł̒���
	float y_vec = (pad.lY - unresponsive_range) / (length - unresponsive_range);

	return false;
}
