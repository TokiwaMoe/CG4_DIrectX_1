#include "GameScene.h"


void GameScene::Initialize(WinApp* winApp, DirectXCommon* dxCommon, Input* input, Audio* audio)
{
	this->dxCommon = dxCommon;
	this->input = input;
	this->audio = audio;
	this->winApp = winApp;

	winApp->Initialize();

	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

#pragma region キー取得
	input = Input::GetInstance();
	input->Initialize(winApp);
	input->MouseInitialize(winApp);
#pragma endregion

#pragma region カメラ
	// カメラ生成
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height, input);
#pragma endregion

#pragma region Object3d
	Object3d::StaticInitialize(dxCommon->GetDev(), camera);
#pragma endregion

#pragma region FBX
	//デバイスをセット
	FbxObject3d::SetDevice(dxCommon->GetDev());
	//カメラをセット
	FbxObject3d::SetCamera(camera);
	//グラフィックパイプライン生成
	FbxObject3d::CreateGraphicsPipline();
	//FBX
	FbxLoader::GetInstance()->Initiallize(dxCommon->GetDev());
#pragma endregion

#pragma region スプライト
	Sprite::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);

	Sprite::LoadTexture(debugTextTexNumber, L"Resource/debugfon.png");
	Sprite::LoadTexture(1, L"Resource/back.png");

	debugText.Initialize(debugTextTexNumber);
#pragma endregion

#pragma region サウンド
	Audio* audio = nullptr;
	audio = new Audio;
	audio->Initialize();
#pragma endregion
}

void GameScene::Update()
{
	//DirectX毎フレーム処理　ここから
	input->KeyUpdate();
	input->MouseUpdate();
	//マウス座標
	POINT mousePos;
	GetCursorPos(&mousePos);
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

	//std::ostringstream debugstr;

	//const XMFLOAT3& cameraPos = camera->GetEye();
	//debugstr << "cameraPo("
	//	<< std::fixed << std::setprecision(2)
	//	<< cameraPos.x << ","
	//	<< cameraPos.y << ","
	//	<< cameraPos.z << ")",
	//	debugText.Print(debugstr.str(), 50, 70, 1.0f);
	//camera->Update();
	//particleMan->Update();

	//objSkydome->Update();
	//objGround->Update();
	//objFighter->Update();
	//objSphere->Update();
	//objSphere2->Update();
	//object1->Update();

	////background->SetColor({ 0,0,0,1 });
	//background->Update();
}

void GameScene::Draw()
{
#pragma region 背景スプライト描画
	Sprite::PreDraw(dxCommon->GetCmdList());

	//background->Draw();

	Sprite::PostDraw();
	//深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3D描画
	Object3d::PreDraw(dxCommon->GetCmdList());

	/*objSkydome->Draw();
	objGround->Draw();
	objFighter->Draw();
	objSphere->Draw();
	objSphere2->Draw();*/
	//object1->Draw(dxCommon->GetCmdList());

	Object3d::PostDraw();

	// パーティクルの描画
	particleMan->Draw(dxCommon->GetCmdList());
#pragma endregion

#pragma region 前景スプライト描画


	Sprite::PreDraw(dxCommon->GetCmdList());

	/*char str[256];
	sprintf_s(str, "60");
	debugText.Print(str, WinApp::window_width / 2 - 70, 10, 8.0f);*/
	debugText.DrawAll(dxCommon->GetCmdList());
	Sprite::PostDraw();

	dxCommon->PreDraw();
	dxCommon->PostDraw();
}

void GameScene::Delete()
{
	FbxLoader::GetInstance()->Finalize();
	//DirectX解放
	delete dxCommon;
}
