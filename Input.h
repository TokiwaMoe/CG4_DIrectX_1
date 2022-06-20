#pragma once
#include<wrl.h>
#define DIRECTINPUT_VERSION		0x0800	//DirectInput�̃o�[�W�����w��
#include<dinput.h>
#include"WinApp.h"

class Input 
{
public:
	//�}�E�X
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
	//namespace�ȗ�
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
private:
	//WindowAPI
	WinApp* winApp = nullptr;
public://�����o�֐�
	//������
	void Initialize(WinApp *winApp);
	void MouseInitialize(WinApp* winApp);
	//�X�V
	void KeyUpdate();
	void MouseUpdate();

/// <summary>
/// �L�[�̉������`�F�b�N
/// </summary>
/// <parm name="keyNumber">�L�[�ԍ�(DIK_0 ��)</param>
/// <return>������Ă��邩</returns>
	bool PushKey(BYTE keyNumber);
	bool PushMouse(int MouseNumber);

/// <summary>
/// �L�[�̃g���K�[���`�F�b�N
/// </summary>
/// <parm name="keyNumber">�L�[�ԍ�(DIK_0 ��)</param>
/// <return>������Ă��邩</returns>
	bool TriggerKey(BYTE keyNumber);
	bool TriggerMouse(int MouseNumber);

	/// <summary>
	/// �}�E�X�ړ��ʂ��擾
	/// </summary>
	/// <returns>�}�E�X�ړ���</returns>
	MouseMove GetMouseMove();

	static Input *GetInstance();

private: //�����o�ϐ�
	//DirectInput�̃C���X�^���X
	ComPtr<IDirectInput8> dinput;
	//�L�[�{�[�h�̃f�o�C�X
	ComPtr<IDirectInputDevice8> devkeyboard;
	//�}�E�X�̃f�o�C�X
	ComPtr<IDirectInputDevice8> devMouse;
	//�S�L�[���
	BYTE key[256] = {};
	//�O��̃L�[
	BYTE oldkey[256] = {};
	//�}�E�X�̏��
	DIMOUSESTATE mouse = { 0 };
	//�O��̃}�E�X
	DIMOUSESTATE oldMouse = { 0 };
	
};