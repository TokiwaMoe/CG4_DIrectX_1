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
	//メッセージで分岐
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

//# Windouwsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	WinApp* winApp = nullptr;

	//WindowsAPIの初期化
	winApp = new WinApp();
	winApp->Initialize();
	winApp->Finalize();
	winApp->ProcessMessage();

	//ポインタ置き場
	DirectXCommon* dxCommon = nullptr;
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

	//ポインタ置き場
	Input* input = nullptr;
	//入力の初期化
	input = new Input();
	input->Initialize(winApp);
	input->MouseInitialize(winApp);

	// 3Dオブジェクトにカメラをセット
	//Object3d::SetCamera(camera);

	//3Dオブジェクト静的初期化
	Object3d::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);
	//3dオブジェクト生成
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
	objSphere->SetPosition({ -50,0,0 });
	objSphere2->SetPosition({ 0,0,0 });

	//パーティクル
	ParticleManager* particleMan = nullptr;
	particleMan = ParticleManager::Create(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);

	//スプライト
	Sprite::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);
	DebugText debugText;

	int debugTextTexNumber = 0;
	Sprite::LoadTexture(debugTextTexNumber, L"Resource/debugfon.png");

	debugText.Initialize(debugTextTexNumber);

	// 背景スプライト生成
	Sprite* background = nullptr;
	background = Sprite::Create(1, { 0.0f,0.0f });

	Audio* audio = nullptr;
	audio = new Audio;
	audio->Initialize();

	XMFLOAT3 position = objSphere->GetPosition();
	float speed = 1;
	float furiction = 0.5;

	//audio->PlayBGMWave("Resource/BGM.wav", 0.3f, true);
	while (true) {

		//Windowsのメッセージ処理
		if (winApp->ProcessMessage())
		{
			break;
		}

		//DirectX毎フレーム処理　ここから
		input->KeyUpdate();
		input->MouseUpdate();
		dxCommon->PreDraw();

		//マウス座標
		POINT mousePos;
		GetCursorPos(&mousePos);

		//パーティクル---------------------------------------------------------
		//for (int i = 0; i < 10; i++) {
		//	// X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
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

		//	// 追加
		//	particleMan->Add(60, pos, vel, acc, 1.0f, 0.0f);
		//}

		//摩擦--------------------------------------------------------
		position.x += speed;

		speed *= furiction;

		objSphere->SetPosition(position);

		//更新--------------------------------------------------------
		particleMan->Update();

		objSkydome->Update();
		objGround->Update();
		objFighter->Update();
		objSphere->Update();
		objSphere2->Update();

		//// ４．描画コマンドここから

		//dxCommon->GetCmdList()->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, WinApp::window_width, WinApp::window_height));

		//dxCommon->GetCmdList()->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, WinApp::window_width, WinApp::window_height));

		//dxCommon->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

#pragma region 背景スプライト描画
		Sprite::PreDraw(dxCommon->GetCmdList());

		//background->Draw();

		Sprite::PostDraw();
		// 深度バッファクリア
		dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3D描画
		Object3d::PreDraw(dxCommon->GetCmdList());

		objSkydome->Draw();
		//objGround->Draw();
		//objFighter->Draw();
		objSphere->Draw();
		//objSphere2->Draw();

		Object3d::PostDraw();

		// パーティクルの描画
		//particleMan->Draw(dxCommon->GetCmdList());
#pragma endregion

#pragma region 前景スプライト描画
		Sprite::PreDraw(dxCommon->GetCmdList());

		char str[256];
		sprintf_s(str, "frame : %f", position.x);
		debugText.Print(str, 10, 10, 1.0f);

		debugText.DrawAll(dxCommon->GetCmdList());
		Sprite::PostDraw();
#pragma endregion
		// ４．描画コマンドここまで
		dxCommon->PostDraw();

		//DirectX毎フレーム処理　ここまで

		// バッファをフリップ（裏表の入替え）
	}
	/*xAudio2.Reset();
	SoundUnload(&soundData1);*/
	//WindowAPIの終了処理
	winApp->Finalize();
	//WindowAPI解放
	delete winApp;
	//DirectX解放
	delete dxCommon;


	return 0;
}