#include "GameScene.h"


void GameScene::Initialize(WinApp* winApp)
{
	this->dxCommon = dxCommon;
	this->input = input;
	this->audio = audio;

	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

#pragma region キー取得
	input = Input::GetInstance();
	input->Initialize(winApp);
	input->MouseInitialize(winApp);
	input->PadInitialize(winApp);
#pragma endregion

#pragma region カメラ
	// カメラ生成
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height, input);
	// 3Dオブジェクトにカメラをセット
	Object3d::SetCamera(camera);
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

	debugText.Initialize(debugTextTexNumber);
#pragma endregion

#pragma region パーティクル
	particleMan = ParticleManager::Create(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);
#pragma endregion

#pragma region サウンド
	audio = new Audio;
	audio->Initialize();
#pragma endregion

#pragma region ライト
	Light::StaticInitialize(dxCommon->GetDev());
	//ライト生成
	light = Light::Create();
	//ライト色を設定
	light->SetLightColor({ 1,1,1 });
	//3Dオブジェクトにライトをセット
	Object3d::SetLight(light);
#pragma endregion

	// カメラ注視点をセット
	camera->SetTarget({ 0, 2.5f, 0 });
	camera->SetDistance(8.0f);
}

void GameScene::Object3dCreate()
{
	Object3dModelSkydome = Object3dModel::LoadFromOBJ("skydome");
	Object3dModelGround = Object3dModel::LoadFromOBJ("ground");
	Object3dModelSphere = Object3dModel::LoadFromOBJ("sphere");
	Object3dModelSphere2 = Object3dModel::LoadFromOBJ("sphere");

	objSkydome = Object3d::Create();
	objSkydome->InitializeGraphicsPipeline(L"Resource/shaders/OBJVertexShader.hlsl", L"Resource/shaders/OBJPixelShader.hlsl");
	objGround = Object3d::Create();
	objGround->InitializeGraphicsPipeline(L"Resource/shaders/OBJVertexShader.hlsl", L"Resource/shaders/OBJPixelShader.hlsl");
	objSphere = Object3d::Create();
	objSphere->InitializeGraphicsPipeline(L"Resource/shaders/OBJVS_Light.hlsl", L"Resource/shaders/OBJPS_Light.hlsl");
	objSphere2 = Object3d::Create();
	objSphere2->InitializeGraphicsPipeline(L"Resource/shaders/ToonVS.hlsl", L"Resource/shaders/ToonPS.hlsl");

	objSphere->SetPosition({ +10,0,0 });
	objSphere2->SetPosition({ -10,0,0 });
	objSkydome->SetScale({ 3,3,3 });

	//モデル名を指定してファイル読み込み
	model1 = FbxLoader::GetInstance()->LoadMadelFromFile("boneTest");
	object1 = new FbxObject3d;
	object1->Initialize();
	object1->SetModel(model1);
	object1->PlayAnimation();
	object1->SetRotation({ 0, 90, 0 });
}

void GameScene::Resource2dCreate()
{
	Sprite::LoadTexture(debugTextTexNumber, L"Resource/debugfon.png");
	Sprite::LoadTexture(1, L"Resource/back.png");

	background = Sprite::Create(1, { 0.0f,0.0f });
}

void GameScene::GameInitialize()
{
	player->Initialize();
	enemy->Initialize();
	sword->Initialize();
	skill->Initialize();
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

void GameScene::ResourcesUpdate()
{
}

void GameScene::GameUpdate()
{
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

void GameScene::ResourceDraw()
{
}

void GameScene::GameDraw()
{
}

void GameScene::Delete()
{
	FbxLoader::GetInstance()->Finalize();
	//DirectX解放
	delete dxCommon;
}
