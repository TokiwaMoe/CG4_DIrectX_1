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
#include"Object3dModel.h"
#include"Sprite.h"
#include"DebugText.h"
#include"Collision.h"
#include"Audio.h"
#include"ParticleManager.h"
#include"DebugCamera.h"
#include"fbxsdk.h"
#include"FbxLoader.h"
#include"FbxObject3d.h"
#include"PostEffect.h"
#include"Light.h"

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
	FbxManager* fbxManager = FbxManager::Create();

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
	Input* input = Input::GetInstance();
	input->Initialize(winApp);
	input->MouseInitialize(winApp);

	DebugCamera* camera = nullptr;
	// �J��������
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height, input);

	// 3D�I�u�W�F�N�g�ɃJ�������Z�b�g
	Object3d::SetCamera(camera);

	//3D�I�u�W�F�N�g�ÓI������
	Object3d::StaticInitialize(dxCommon->GetDev(), camera);
	//3d�I�u�W�F�N�g����
	Object3dModel* Object3dModelSkydome = Object3dModel::LoadFromOBJ("skydome");
	Object3dModel* Object3dModelGround = Object3dModel::LoadFromOBJ("ground");
	Object3dModel* Object3dModelFighter = Object3dModel::LoadFromOBJ("chr_sword");
	Object3dModel* Object3dModelSphere = Object3dModel::LoadFromOBJ("sphere");
	Object3dModel* Object3dModelSphere2 = Object3dModel::LoadFromOBJ("sphere");

	Object3d* objSkydome = Object3d::Create();
	Object3d* objGround = Object3d::Create();
	Object3d* objFighter = Object3d::Create();
	Object3d* objSphere = Object3d::Create();
	Object3d* objSphere2 = Object3d::Create();

	objSkydome->SetObject3dModel(Object3dModelSkydome);
	objGround->SetObject3dModel(Object3dModelGround);
	objFighter->SetObject3dModel(Object3dModelFighter);
	objSphere->SetObject3dModel(Object3dModelSphere);
	objSphere2->SetObject3dModel(Object3dModelSphere2);

	objFighter->SetPosition({ +1,0,0 });
	objSphere->SetPosition({ +1,1,0 });
	objSphere2->SetPosition({ -1,1,0 });
	// �J���������_���Z�b�g
	camera->SetTarget({ 0, 1, 0 });
	camera->SetDistance(8.0f);

	FbxModel* model1 = nullptr;
	FbxObject3d* object1 = nullptr;

	//�f�o�C�X���Z�b�g
	FbxObject3d::SetDevice(dxCommon->GetDev());
	//�J�������Z�b�g
	FbxObject3d::SetCamera(camera);
	//�O���t�B�b�N�p�C�v���C������
	FbxObject3d::CreateGraphicsPipline();
	//FBX
	FbxLoader::GetInstance()->Initiallize(dxCommon->GetDev());
	//���f�������w�肵�ăt�@�C���ǂݍ���
	model1 = FbxLoader::GetInstance()->LoadMadelFromFile("boneTest");

	object1 = new FbxObject3d;
	object1->Initialize();
	object1->SetModel(model1);
	object1->PlayAnimation();
	object1->SetRotation({ 0, 90, 0 });

	//�p�[�e�B�N��
	ParticleManager* particleMan = nullptr;
	particleMan = ParticleManager::Create(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);

	//�X�v���C�g
	Sprite::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);
	DebugText debugText;

	int debugTextTexNumber = 0;
	Sprite::LoadTexture(debugTextTexNumber, L"Resource/debugfon.png");
	Sprite::LoadTexture(1, L"Resource/back.png");

	debugText.Initialize(debugTextTexNumber);

	// �w�i�X�v���C�g����
	Sprite* background = nullptr;
	background = Sprite::Create(1, { 0.0f,0.0f });

	//�|�X�g�G�t�F�N�g
	PostEffect* postEffect = nullptr;
	//Sprite::LoadTexture(100, L"Resource/background.png");
	//�|�X�g�G�t�F�N�g�̏�����
	postEffect = new PostEffect(input);
	postEffect->Initialize();

	Audio* audio = nullptr;
	audio = new Audio;
	audio->Initialize();

	Light::StaticInitialize(dxCommon->GetDev());
	Light* light = nullptr;
	//���C�g����
	light = Light::Create();
	//���C�g�F��ݒ�
	light->SetLightColor({ 1,1,1 });
	//3D�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	Object3d::SetLight(light);

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
		//�}�E�X���W
		POINT mousePos;
		GetCursorPos(&mousePos);

		//audio->PlayWave("Resources/Alarm01.wav");
	//�I�u�W�F�N�g�̉�]
		{
			/*XMFLOAT3 rot = objSphere->GetRotation();
			rot.y += 1.0f;
			objSphere->SetRotation(rot);
			objSphere2->SetRotation(rot);*/
		}

		{
			//��������������
			static XMVECTOR lightDir = { 0,1,5,0 };

			if (input->PushKey(DIK_W)) { lightDir.m128_f32[1] += 1.0f; }
			else if (input->PushKey(DIK_S)) { lightDir.m128_f32[1] -= 1.0f; }
			if (input->PushKey(DIK_D)) { lightDir.m128_f32[0] += 1.0f; }
			else if (input->PushKey(DIK_A)) { lightDir.m128_f32[0] -= 1.0f; }

			light->SetLightDir(lightDir);

			std::ostringstream debugstr;
			debugstr << "lightDirFactor("
				<< std::fixed << std::setprecision(2)
				<< lightDir.m128_f32[0] << ","
				<< lightDir.m128_f32[1] << ","
				<< lightDir.m128_f32[2] << ")",
				debugText.Print(debugstr.str(), 50, 50, 1.0f);

			debugstr.str("");
			debugstr.clear();

			const XMFLOAT3& cameraPos = camera->GetEye();
			debugstr << "cameraPo("
				<< std::fixed << std::setprecision(2)
				<< cameraPos.x << ","
				<< cameraPos.y << ","
				<< cameraPos.z << ")",
				debugText.Print(debugstr.str(), 50, 70, 1.0f);
		}
		

		for (int i = 0; i < 10; i++) {
			// X,Y,Z�S��[-5.0f,+5.0f]�Ń����_���ɕ��z
			const float rnd_pos = 10.0f;
			XMFLOAT3 pos{};
			pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
			pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
			pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

			const float rnd_vel = 0.1f;
			XMFLOAT3 vel{};
			vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
			vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
			vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

			XMFLOAT3 acc{};
			const float rnd_acc = 0.001f;
			acc.y = -(float)rand() / RAND_MAX * rnd_acc;

			// �ǉ�
			particleMan->Add(60, pos, vel, acc, 1.0f, 0.0f);
		}

		std::ostringstream debugstr;

		const XMFLOAT3& cameraPos = camera->GetEye();
		debugstr << "cameraPo("
			<< std::fixed << std::setprecision(2)
			<< cameraPos.x << ","
			<< cameraPos.y << ","
			<< cameraPos.z << ")",
			debugText.Print(debugstr.str(), 50, 70, 1.0f);

		camera->Update();
		particleMan->Update();
		light->Update();
		objSkydome->Update();
		objGround->Update();
		objFighter->Update();
		objSphere->Update();
		objSphere2->Update();
		object1->Update();

		//background->SetColor({ 0,0,0,1 });
		background->Update();

		postEffect->PreDrawScene(dxCommon->GetCmdList());
#pragma region �w�i�X�v���C�g�`��
		Sprite::PreDraw(dxCommon->GetCmdList());

		//background->Draw();

		Sprite::PostDraw();
		 //�[�x�o�b�t�@�N���A
		dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3D�`��
		Object3d::PreDraw(dxCommon->GetCmdList());

		objSkydome->Draw();
		objGround->Draw();
		//objFighter->Draw();
		objSphere->Draw();
		objSphere2->Draw();
		object1->Draw(dxCommon->GetCmdList());

		Object3d::PostDraw();

		// �p�[�e�B�N���̕`��
		//particleMan->Draw(dxCommon->GetCmdList());
#pragma endregion

#pragma region �O�i�X�v���C�g�`��
		

		Sprite::PreDraw(dxCommon->GetCmdList());

		/*char str[256];
		sprintf_s(str, "60");
		debugText.Print(str, WinApp::window_width / 2 - 70, 10, 8.0f);*/
		debugText.DrawAll(dxCommon->GetCmdList());
		Sprite::PostDraw();

		
		postEffect->PostDrawScene(dxCommon->GetCmdList());

		
#pragma endregion
		dxCommon->PreDraw();
		postEffect->Draw(dxCommon->GetCmdList());
		
		// �S�D�`��R�}���h�����܂�
		dxCommon->PostDraw();

		//DirectX���t���[�������@�����܂�

		// �o�b�t�@���t���b�v�i���\�̓��ւ��j
	}
	/*xAudio2.Reset();
	SoundUnload(&soundData1);*/
	//WindowAPI�̏I������
	winApp->Finalize();
	FbxLoader::GetInstance()->Finalize();
	//WindowAPI���
	delete winApp;
	//DirectX���
	delete dxCommon;
	delete object1;
	//delete Object3dModel1;
	delete postEffect;


	return 0;
}