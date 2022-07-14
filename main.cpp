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
#include"Player.h"
#include"Enemy.h"

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
	FbxManager* fbxManager = FbxManager::Create();

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
	Input* input = Input::GetInstance();
	input->Initialize(winApp);
	input->MouseInitialize(winApp);

	DebugCamera* camera = nullptr;
	// カメラ生成
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height, input);

	// 3Dオブジェクトにカメラをセット
	Object3d::SetCamera(camera);

	//3Dオブジェクト静的初期化
	Object3d::StaticInitialize(dxCommon->GetDev(), camera);
	//3dオブジェクト生成
	Object3dModel* Object3dModelSkydome = Object3dModel::LoadFromOBJ("skydome");
	Object3dModel* Object3dModelGround = Object3dModel::LoadFromOBJ("ground");
	Object3dModel* Object3dModelSphere = Object3dModel::LoadFromOBJ("sphere");
	Object3dModel* Object3dModelSphere2 = Object3dModel::LoadFromOBJ("sphere");

	Object3d* objSkydome = Object3d::Create();
	objSkydome->InitializeGraphicsPipeline(L"Resource/shaders/OBJVertexShader.hlsl", L"Resource/shaders/OBJPixelShader.hlsl");
	Object3d* objGround = Object3d::Create();
	objGround->InitializeGraphicsPipeline(L"Resource/shaders/OBJVertexShader.hlsl", L"Resource/shaders/OBJPixelShader.hlsl");
	Object3d* objSphere = Object3d::Create();
	objSphere->InitializeGraphicsPipeline(L"Resource/shaders/ToonVS.hlsl", L"Resource/shaders/ToonPS.hlsl");
	Object3d* objSphere2 = Object3d::Create();
	objSphere2->InitializeGraphicsPipeline(L"Resource/shaders/ToonVS.hlsl", L"Resource/shaders/ToonPS.hlsl");

	objSkydome->SetObject3dModel(Object3dModelSkydome);
	objGround->SetObject3dModel(Object3dModelGround);
	objSphere->SetObject3dModel(Object3dModelSphere);
	objSphere2->SetObject3dModel(Object3dModelSphere2);

	XMFLOAT3 playerPosition = { 1,0,0 };
	objSphere->SetPosition({ +1,0,0 });
	objSphere2->SetPosition({ -10,0,0 });
	objSkydome->SetScale({ 3,3,3 });
	

	FbxModel* model1 = nullptr;
	FbxObject3d* object1 = nullptr;

	//デバイスをセット
	FbxObject3d::SetDevice(dxCommon->GetDev());
	//カメラをセット
	FbxObject3d::SetCamera(camera);
	//グラフィックパイプライン生成
	FbxObject3d::CreateGraphicsPipline();
	//FBX
	FbxLoader::GetInstance()->Initiallize(dxCommon->GetDev());
	//モデル名を指定してファイル読み込み
	model1 = FbxLoader::GetInstance()->LoadMadelFromFile("boneTest");

	object1 = new FbxObject3d;
	object1->Initialize();
	object1->SetModel(model1);
	object1->PlayAnimation();
	object1->SetRotation({ 0, 90, 0 });

	//パーティクル
	ParticleManager* particleMan = nullptr;
	particleMan = ParticleManager::Create(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);

	//スプライト
	Sprite::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);
	DebugText debugText;

	int debugTextTexNumber = 0;
	Sprite::LoadTexture(debugTextTexNumber, L"Resource/debugfont.png");
	Sprite::LoadTexture(1, L"Resource/Back.png");

	debugText.Initialize(debugTextTexNumber);

	// 背景スプライト生成
	Sprite* background = nullptr;
	background = Sprite::Create(1, { 0.0f,0.0f });

	//ポストエフェクト
	PostEffect* postEffect = nullptr;
	//ポストエフェクトの初期化
	postEffect = new PostEffect(input);
	postEffect->Initialize(L"Resource/shaders/PostEffectVS.hlsl", L"Resource/shaders/PostEffectPS.hlsl");
	

	Audio* audio = nullptr;
	audio = new Audio;
	audio->Initialize();

	Light::StaticInitialize(dxCommon->GetDev());
	Light* light = nullptr;
	//ライト生成
	light = Light::Create();
	//ライト色を設定
	light->SetLightColor({ 1,1,1 });
	//3Dオブジェクトにライトをセット
	Object3d::SetLight(light);

	Player* player = new Player();
	player->Initialize();
	// カメラ注視点をセット
	camera->SetTarget({0, 2.5f, 0});
	camera->SetDistance(8.0f);

	Enemy* enemy = new Enemy();
	enemy->Initialize();

	int postEffectFlag = 0;
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
		//マウス座標
		POINT mousePos;
		GetCursorPos(&mousePos);

		//audio->PlayWave("Resources/Alarm01.wav");
	//オブジェクトの回転
		{
			/*XMFLOAT3 rot = objSphere->GetRotation();
			rot.y += 1.0f;
			objSphere->SetRotation(rot);
			objSphere2->SetRotation(rot);*/
		}

		//{
		//	//光線方向初期化
		//	static XMVECTOR lightDir = { 0,1.0f,5.0f,0 };

		//	if (input->PushKey(DIK_W)) { lightDir.m128_f32[1] += 1.0f; }
		//	else if (input->PushKey(DIK_S)) { lightDir.m128_f32[1] -= 1.0f; }
		//	if (input->PushKey(DIK_D)) { lightDir.m128_f32[0] += 1.0f; }
		//	else if (input->PushKey(DIK_A)) { lightDir.m128_f32[0] -= 1.0f; }

		//	light->SetLightDir(lightDir);
		//}
		

		for (int i = 0; i < 10; i++) {
			// X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
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

			// 追加
			particleMan->Add(60, pos, vel, acc, 1.0f, 0.0f);
		}

		enemy->Update();
		player->Update(camera);

		camera->Update();
		particleMan->Update();
		light->Update();
		objSkydome->Update();
		objGround->Update();
		objSphere->Update();
		objSphere2->Update();
		object1->Update();

		//background->SetColor({ 0,0,0,1 });
		background->Update();

		if (input->TriggerKey(DIK_RIGHT))
		{
			postEffectFlag++;
		}
		if (input->TriggerKey(DIK_LEFT))
		{
			postEffectFlag--;
		}

		switch (postEffectFlag)
		{
		case 1:
			postEffect->Initialize(L"Resource/shaders/RGBShiftVS.hlsl", L"Resource/shaders/RGBShiftPS.hlsl");
			break;
		case 2:
			postEffect->Initialize(L"Resource/shaders/sepiaVS.hlsl", L"Resource/shaders/sepiaPS.hlsl");
			break;
		case 3:
			postEffect->Initialize(L"Resource/shaders/monochromeVS.hlsl", L"Resource/shaders/monochromePS.hlsl");
			break;
		case 4:
			postEffect->Initialize(L"Resource/shaders/negapoziVS.hlsl", L"Resource/shaders/negapoziPS.hlsl");
			break;
		case 5:
			postEffect->Initialize(L"Resource/shaders/noizeVS.hlsl", L"Resource/shaders/noizePS.hlsl");
			break;
		case 6:
			postEffect->Initialize(L"Resource/shaders/noize_2VS.hlsl", L"Resource/shaders/noize_2PS.hlsl");
			break;
		case 7:
			postEffect->Initialize(L"Resource/shaders/WhiteNoizeVS.hlsl", L"Resource/shaders/WhiteNoizePS.hlsl");
			break;
		case 8:
			postEffect->Initialize(L"Resource/shaders/mosaicVS.hlsl", L"Resource/shaders/mosaicPS.hlsl");
			break;
		case 9:
			postEffect->Initialize(L"Resource/shaders/VignetteVS.hlsl", L"Resource/shaders/VignettePS.hlsl");
			break;
		case 10:
			postEffect->Initialize(L"Resource/shaders/GaussianVS.hlsl", L"Resource/shaders/GaussianPS.hlsl");
			break;
		case 11:
			postEffect->Initialize(L"Resource/shaders/DepthVS.hlsl", L"Resource/shaders/DepthPS.hlsl");
			break;
		case 12:
			postEffect->Initialize(L"Resource/shaders/bloomVS.hlsl", L"Resource/shaders/bloomPS.hlsl");
			break;
		default:
			postEffect->Initialize(L"Resource/shaders/PostEffectVS.hlsl", L"Resource/shaders/PostEffectPS.hlsl");
			postEffectFlag = 0;
			break;
		}

		

		postEffect->PreDrawScene(dxCommon->GetCmdList());
#pragma region 3D描画
		
		Object3d::PreDraw(dxCommon->GetCmdList());

		player->Draw();
		enemy->Draw();
		objSkydome->Draw();
		objGround->Draw();
		//objFighter->Draw();
		objSphere->Draw();
		//objSphere2->Draw();
		object1->Draw(dxCommon->GetCmdList());

		Object3d::PostDraw();
		// パーティクルの描画
		//particleMan->Draw(dxCommon->GetCmdList());
#pragma endregion
		postEffect->PostDrawScene(dxCommon->GetCmdList());

		dxCommon->PreDraw();
#pragma region 背景スプライト描画
		Sprite::PreDraw(dxCommon->GetCmdList());

		background->Draw();

		Sprite::PostDraw();
		//深度バッファクリア
		dxCommon->ClearDepthBuffer();
#pragma endregion

		postEffect->Draw(dxCommon->GetCmdList());

#pragma region 前景スプライト描画
		Sprite::PreDraw(dxCommon->GetCmdList());

		char str[256];
		sprintf_s(str, "60");
		debugText.Print(str, WinApp::window_width / 2 - 70, 10, 8.0f);
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
	FbxLoader::GetInstance()->Finalize();
	//WindowAPI解放
	delete winApp;
	//DirectX解放
	delete dxCommon;
	delete object1;
	//delete Object3dModel1;
	delete postEffect;


	return 0;
}