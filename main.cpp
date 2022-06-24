#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include <string>
#include <cassert>
#include<sstream>
#include<iomanip>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#define DIRECTINPUT_VERSION   0x0800
#include <dinput.h>
#include <math.h>
#include <DirectXTex.h>
#include<wrl.h>
#include<d3dx12.h>
#include<xaudio2.h>
#include<fstream>
#include"Input.h"
#include"WinApp.h"
#include"DirectXCommon.h"
#include"Object3d.h"
#include"Model.h"
#include"Sprite.h"
#include"DebugText.h"
#include"Collision.h"
#include"Audio.h"
#include"ParticleManager.h"
#include"DebugCamera.h"

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib, "xaudio2.lib")

using namespace DirectX;
using namespace Microsoft::WRL;


LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	//���b�Z�[�W�ŕ���
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

//# Windouws�A�v���ł̃G���g���[�|�C���g(main�֐�)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	WinApp* winApp = nullptr;

	//WindowsAPI�̏�����
	winApp = new WinApp();
	winApp->Initialize();
	winApp->Finalize();
	winApp->ProcessMessage();

	//�|�C���^�u����
	DirectXCommon* dxCommon = nullptr;
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

	//�|�C���^�u����
	Input* input = nullptr;
	//���͂̏�����
	input = new Input();
	input->Initialize(winApp);
	input->MouseInitialize(winApp);

	// 3D�I�u�W�F�N�g�ɃJ�������Z�b�g
	//Object3d::SetCamera(camera);

	//3D�I�u�W�F�N�g�ÓI������
	Object3d::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);
	//3d�I�u�W�F�N�g����
	Model* modelSkydome = Model::LoadFromOBJ("skydome");
	Model* modelGround = Model::LoadFromOBJ("ground");
	Model* modelFighter = Model::LoadFromOBJ("chr_sword");
	Model* modelSphere = Model::LoadFromOBJ("sphere");
	Model* modelSphere2 = Model::LoadFromOBJ("sphere");

	Object3d* objSkydome = Object3d::Create();
	Object3d* objGround = Object3d::Create();
	Object3d* objFighter = Object3d::Create();
	Object3d* objSphere = Object3d::Create();
	Object3d* objSphere2 = Object3d::Create();

	objSkydome->SetModel(modelSkydome);
	objGround->SetModel(modelGround);
	objFighter->SetModel(modelFighter);
	objSphere->SetModel(modelSphere);
	objSphere2->SetModel(modelSphere2);

	objFighter->SetPosition({ +1,0,0 });
	objSphere->SetScale({ 1.5,1.5,1.5 });
	objSphere->SetPosition({ -20,0,0 });
	objSphere2->SetPosition({ 20,0,0 });
	objSphere2->SetScale({ 1.5,1.5,1.5 });
	objSphere2->SetColor({ 1,0,0,1 });

	//�p�[�e�B�N��
	ParticleManager* particleMan = nullptr;
	particleMan = ParticleManager::Create(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);

	//�X�v���C�g
	Sprite::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);
	DebugText debugText;

	int debugTextTexNumber = 0;
	Sprite::LoadTexture(debugTextTexNumber, L"Resource/debugfon.png");

	debugText.Initialize(debugTextTexNumber);

	// �w�i�X�v���C�g����
	Sprite* background = nullptr;
	background = Sprite::Create(1, { 0.0f,0.0f });

	Audio* audio = nullptr;
	audio = new Audio;
	audio->Initialize();

	XMFLOAT3 position = objSphere->GetPosition();
	XMFLOAT3 position2 = objSphere2->GetPosition();
	/*float vy = 3.0f;
	float speed = 6;
	float furiction = 0.8;
	float weight = 30;*/
	float gravity = 9.8 / 60.0f;
	float speed = 2;
	float time = 0;
	float angle = 30;
	float PI = 3.141592;
	float k = 2;
	float m = 1;

	float m1 = 0.5;
	float m2 = 0.45;
	float v1 = 1;
	float v2 = 1;
	bool hitFlag = false;
	bool repulsion = false;

	//audio->PlayBGMWave("Resource/BGM.wav", 0.3f, true);
	while (true) {

		//Windows�̃��b�Z�[�W����
		if (winApp->ProcessMessage())
		{
			break;
		}

		//DirectX���t���[�������@��������
		input->KeyUpdate();
		input->MouseUpdate();
		dxCommon->PreDraw();

		//�}�E�X���W
		POINT mousePos;
		GetCursorPos(&mousePos);

		//�p�[�e�B�N��---------------------------------------------------------
		//for (int i = 0; i < 10; i++) {
		//	// X,Y,Z�S��[-5.0f,+5.0f]�Ń����_���ɕ��z
		//	const float rnd_pos = 10.0f;
		//	XMFLOAT3 pos{};
		//	pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		//	pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		//	pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

		//	const float rnd_vel = 0.1f;
		//	XMFLOAT3 vel{};
		//	vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		//	vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		//	vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		//	XMFLOAT3 acc{};
		//	const float rnd_acc = 0.001f;
		//	acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		//	// �ǉ�
		//	particleMan->Add(60, pos, vel, acc, 1.0f, 0.0f);
		//}

		//�d��-------------------------------------------------------
		/*vy -= gravity;
		position.y += vy;*/

		//���C--------------------------------------------------------
		/*float N = weight * gravity;
		float friction_power = N * furiction;
		float acceleration = friction_power / weight;
		position.x += speed;
		speed -= acceleration;

		if (speed <= 0)
		{
			speed = 0;
		}*/

		//��C��R-----------------------------------------------------
		/*float dy = speed * time;
		float dx = speed * time;
		float g = k * speed / m;
		float vx = dx * cos((PI / 180) * angle);
		float vy = dy * sin((PI / 180) * angle) - 0.5 * (g * (time * time));
		float vy2 = dy * sin((PI / 180) * angle) - 0.5 * (gravity * (time * time));
		position.x = vx;
		position.y = vy;

		position2.x = vx;
		position2.y = vy2;

		time++;*/

		//����---------------------------------------------------------

		
		if (input->PushKey(DIK_SPACE))
		{
			position.x = -20;
			position2.x = 0;
			repulsion = false;
			hitFlag = false;
		}
		else {
			hitFlag = Collision::CheckSphere2(position, position2, 1.0f, 1.0f);

			if (hitFlag)
			{
				char str2[256];
				sprintf_s(str2, "HIT");
				debugText.Print(str2, 10, 90, 1.0f);
				repulsion = true;
			}

			if (repulsion)
			{
				float a2 = m1 * v1 + m2 * v2;
				float law2 = a2 / m2;
				position2.x += law2;
				float a1 = m1 * v1 + m2 * v2;
				float law1 = a1 / m1;
				position.x -= law1;

				char str2[256];
				sprintf_s(str2, "b1 : %f b2 : %f", law1, law2);
				debugText.Print(str2, 10, 60, 1.0f);
			}
			else {
				position.x += v1;
				position2.x -= v2;
			}

			objSphere->SetPosition(position);
			objSphere2->SetPosition(position2);
		}


		//�X�V--------------------------------------------------------
		particleMan->Update();

		objSkydome->Update();
		objGround->Update();
		objFighter->Update();
		objSphere->Update();
		objSphere2->Update();

		//// �S�D�`��R�}���h��������

		//dxCommon->GetCmdList()->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, WinApp::window_width, WinApp::window_height));

		//dxCommon->GetCmdList()->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, WinApp::window_width, WinApp::window_height));

		//dxCommon->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

#pragma region �w�i�X�v���C�g�`��
		Sprite::PreDraw(dxCommon->GetCmdList());

		//background->Draw();

		Sprite::PostDraw();
		// �[�x�o�b�t�@�N���A
		dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3D�`��
		Object3d::PreDraw(dxCommon->GetCmdList());

		objSkydome->Draw();
		//objGround->Draw();
		//objFighter->Draw();
		objSphere->Draw();
		objSphere2->Draw();

		Object3d::PostDraw();

		// �p�[�e�B�N���̕`��
		//particleMan->Draw(dxCommon->GetCmdList());
#pragma endregion

#pragma region �O�i�X�v���C�g�`��
		Sprite::PreDraw(dxCommon->GetCmdList());

		char str[256];
		sprintf_s(str, "obj1 : %f %f %f", position.x, position.y, position.z);
		debugText.Print(str, 10, 10, 1.0f);

		char str3[256];
		sprintf_s(str, "obj2 : %f %f %f", position2.x, position2.y, position2.z);
		debugText.Print(str, 10, 30, 1.0f);

		debugText.DrawAll(dxCommon->GetCmdList());
		Sprite::PostDraw();
#pragma endregion
		// �S�D�`��R�}���h�����܂�
		dxCommon->PostDraw();

		//DirectX���t���[�������@�����܂�

		// �o�b�t�@���t���b�v�i���\�̓��ւ��j
	}
	/*xAudio2.Reset();
	SoundUnload(&soundData1);*/
	//WindowAPI�̏I������
	winApp->Finalize();
	//WindowAPI���
	delete winApp;
	//DirectX���
	delete dxCommon;


	return 0;
}