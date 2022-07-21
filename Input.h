#pragma once
#include<wrl.h>
#define DIRECTINPUT_VERSION		0x0800	//DirectInput�̃o�[�W�����w��
#include<dinput.h>
#include"WinApp.h"
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>

class Input 
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
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

	XMFLOAT2 GetMousePos() { return mousePos; }

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

public:
	XMFLOAT2 mousePos = { 0,0 };
	
};