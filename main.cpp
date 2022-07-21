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
	objSphere->SetPosition({ -20,0,0 });
	objSphere2->SetPosition({ 0,0,0 });
	objSphere2->SetScale({ 1.5,1.5,1.5 });
	objSphere2->SetColor({ 1,0,0,1 });

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

	Sprite* line = nullptr;
	Sprite::LoadTexture(2, L"Resource/Line.png");
	line = Sprite::Create(2, { 0.0f,0.0f });
	

	Sprite* cicre = nullptr;
	Sprite::LoadTexture(3, L"Resource/Cicre.png");
	cicre = Sprite::Create(3, { 10.0f,0.0f });
	


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
	float angle = 5;
	float PI = 3.141592;
	float k = 2;
	float m = 1;

	float m1 = 0.5;
	float m2 = 0.45;
	float v1 = 1;
	float v2 = 1;
	float e1; //反発係数
	float e2;
	bool hitFlag = false;
	bool repulsion = false;

	XMFLOAT2 linePos = { 0,0 };
	XMFLOAT2 b = { 0,0 };

	float stiffness = 0.1;
	float damping = 0.9;
	float vel = 0.0;
	float target = 0;
	float old = 0;
	bool flag = false;

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
		POINT p;
		GetCursorPos(&p);

		ScreenToClient(winApp->GetHwnd(), &p);

		input->mousePos.x = p.x;
		input->mousePos.y = p.y;

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

		//重力-------------------------------------------------------
		/*vy -= gravity;
		position.y += vy;*/

		//摩擦--------------------------------------------------------
		/*float N = weight * gravity;
		float friction_power = N * furiction;
		float acceleration = friction_power / weight;
		position.x += speed;
		speed -= acceleration;

		if (speed <= 0)
		{
			speed = 0;
		}*/

		//空気抵抗-----------------------------------------------------
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

		//反発---------------------------------------------------------
		
		/*if (input->PushKey(DIK_SPACE))
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
				sprintf_s(str2, "e1 : %f e2 : %f", law1, law2);
				debugText.Print(str2, 10, 60, 1.0f);
			}
			else {
				position.x += v1;
				position2.x -= v2;
			}

			objSphere->SetPosition(position);
			objSphere2->SetPosition(position2);
		}*/

		//反発係数----------------------------------------------------
		/*if (input->PushKey(DIK_SPACE))
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
				e2 = v2 / law2;
				position2.x += v2 * e2;
				float a1 = m1 * v1 + m2 * v2;
				float law1 = a1 / m1;
				e1 = v1 / law1;
				position.x -= v1 * e1;

				char str2[256];
				sprintf_s(str2, "e1 : %f e2 : %f", law1, law2);
				debugText.Print(str2, 10, 60, 1.0f);
			}
			else {
				position.x += v1;
				position2.x -= v2;
			}

			objSphere->SetPosition(position);
			objSphere2->SetPosition(position2);
		}*/

		//円運動----------------------------------------------------------
		/*float m1 = 60;
		float m2 = 45;*/

		/*float length = 30.0f;
		float centerX = position2.x;
		float centerY = position2.y;

		float rad = angle / time;

		float radius = angle * PI / 180.0f;

		float add_X = cos(radius) * length;
		float add_Y = sin(radius) * length;

		position.x = centerX + add_X;
		position.y = centerY + add_Y;

		angle += 5;
		time ++;

		objSphere->SetPosition(position);
		objSphere2->SetPosition(position2);*/

		//当たり判定--------------------------------------------------
		line->Initialize();
		cicre->Initialize();

		float cicrePointX = 10.0f + 64.0f / 2;
		float cicrePointY = 64.0f / 2;
		float r = 64.0f / 2;

		float linePointX = linePos.x + 32.0f;
		float linePointY = linePos.y + 32.0f;

		int x = cicrePointX - linePointX;
		int y = cicrePointY - linePointY;
		int dist = sqrtf(pow(x, 2) + pow(y, 2));

		if (dist > r)
		{
			cicre->SetColor({ 1,1,1,1 });
		}
		else {
			cicre->SetColor({ 1,0,0,1 });
		}

		

		if (input->PushKey(DIK_W))
		{
			linePos.y -= 1;
		}
		if (input->PushKey(DIK_S))
		{
			linePos.y += 1;
		}
		if (input->PushKey(DIK_A))
		{
			linePos.x -= 1;
		}
		if (input->PushKey(DIK_D))
		{
			linePos.x += 1;
		}

		line->SetPosition(linePos);

		//連結----------------------------------------------
		line->Initialize();
		cicre->Initialize();

		float force = stiffness * (target - b.y);
		vel = damping * (vel + force);
		b.y += vel;

		input->GetMouseMove();

		if (input->PushMouse(input->M_Left))
		{
			target = input->mousePos.y;
		}

		line->SetSize({ 64, b.y });
		cicre->SetPosition({ 0, b.y });

		//振り子------------------------------------------------------
		//double x;     // 紐を伸ばして一周させた場合に出来る円周上の座標、０は紐が軸の真下にいる位置
		//double speed; // xの速度
		//double angle;
		//int    jiku_x = 320, jiku_y = 100; // 軸の位置
		//int nx, ny;

		//// 初期位置は軸の真下から左方向に45度傾いた位置
		//x = CLENGTH / 8.0;

		//// 初速度は０
		//speed = 0.0;

		//// 公式に従って速度を加算
		//	// MASSの値を小さくするとゆっくり動く
		//speed += -MASS * G * sin(x / LENGTH);

		//// 速度に従って円上の座標を変更
		//x += speed;

		//// 軸を原点とした場合のぶら下がっている物の座標を算出
		//// このままだと－45～45度の振り子になるので
		//// 時計回りに90度（PI/2.0）回転
		//angle = x / LENGTH + PI / 2.0;

		//// 求めた角度から軸を原点とした円周上の座標を取得
		//nx = cos(angle) * LENGTH;
		//ny = sin(angle) * LENGTH;

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
		objSphere2->Draw();

		Object3d::PostDraw();

		// パーティクルの描画
		//particleMan->Draw(dxCommon->GetCmdList());
#pragma endregion

#pragma region 前景スプライト描画
		Sprite::PreDraw(dxCommon->GetCmdList());

		char str[256];
		sprintf_s(str, "%f %f b.y : %f %f", force, vel, b.y, target);
		debugText.Print(str, 10, 10, 1.0f);

		char str3[256];
		sprintf_s(str, "obj2 : %f %f", input->mousePos.x, input->mousePos.y);
		debugText.Print(str, 10, 30, 1.0f);

		cicre->Draw();
		line->Draw();
		

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