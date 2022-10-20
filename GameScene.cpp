#include "GameScene.h"
#include"Input.h"

GameScene::GameScene()
{

}

GameScene::~GameScene(){}

void GameScene::Initialize(DirectXCommon* dxc, Audio* sound)
{
	
	this->dxCommon = dxc;
	this->audio = sound;

#pragma region カメラ
	// カメラ生成
	camera = new Camera(WinApp::window_width, WinApp::window_height);
	// 3Dオブジェクトにカメラをセット
	Object3d::SetCamera(camera);
#pragma endregion

#pragma region Object3d
	Object3d::StaticInitialize(dxCommon->GetDev(), camera);
	modelScene1 = Object3dModel::LoadFromOBJ("scene1");
	modelScene2 = Object3dModel::LoadFromOBJ("scene2");
	modelScene3 = Object3dModel::LoadFromOBJ("scene3");
	modelScene4 = Object3dModel::LoadFromOBJ("scene4");
	Object3dModelGround = Object3dModel::LoadFromOBJ("ground");
	Object3dModelSphere = Object3dModel::LoadFromOBJ("sphere");
	Object3dModelSphere2 = Object3dModel::LoadFromOBJ("sphere");

	objScene1 = Object3d::Create();
	objScene1->InitializeGraphicsPipeline(L"Resource/shaders/OBJVertexShader.hlsl", L"Resource/shaders/OBJPixelShader.hlsl");
	objScene1->SetObject3dModel(modelScene1);
	objScene2 = Object3d::Create();
	objScene2->InitializeGraphicsPipeline(L"Resource/shaders/OBJVertexShader.hlsl", L"Resource/shaders/OBJPixelShader.hlsl");
	objScene2->SetObject3dModel(modelScene2);
	objScene3 = Object3d::Create();
	objScene3->InitializeGraphicsPipeline(L"Resource/shaders/OBJVertexShader.hlsl", L"Resource/shaders/OBJPixelShader.hlsl");
	objScene3->SetObject3dModel(modelScene3);
	objScene4 = Object3d::Create();
	objScene4->InitializeGraphicsPipeline(L"Resource/shaders/OBJVertexShader.hlsl", L"Resource/shaders/OBJPixelShader.hlsl");
	objScene4->SetObject3dModel(modelScene4);
	objSphere = Object3d::Create();
	objSphere->InitializeGraphicsPipeline(L"Resource/shaders/OBJVS_Light.hlsl", L"Resource/shaders/OBJPS_Light.hlsl");
	objSphere->SetObject3dModel(Object3dModelSphere);
	objSphere2 = Object3d::Create();
	objSphere2->InitializeGraphicsPipeline(L"Resource/shaders/ToonVS.hlsl", L"Resource/shaders/ToonPS.hlsl");
	objSphere2->SetObject3dModel(Object3dModelSphere2);

	objSphere->SetPosition({ +10,0,0 });
	objSphere2->SetPosition({ -10,0,0 });
	objScene1->SetScale({ 0.25,0.25,0.25 });
	objScene2->SetScale({ 0.25,0.25,0.25 });
	objScene3->SetScale({ 0.25,0.25,0.25 });
	objScene4->SetScale({ 0.25,0.25,0.25 });

	
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
	//モデル名を指定してファイル読み込み
	model1 = FbxLoader::GetInstance()->LoadMadelFromFile("Player_Attack");
	object1 = new FbxObject3d;
	object1->Initialize();
	object1->SetModel(model1.get());
	object1->PlayAnimation();
	object1->SetRotation({ 0, 0, 0 });
	object1->SetScale({ 0.001,0.001,0.001 });
	object1->SetPosition({ 0,0, 5.5 });
#pragma endregion

#pragma region スプライト
	Sprite::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);

	debugText.Initialize(debugTextTexNumber);
#pragma endregion

#pragma region パーティクル
	particleMan = ParticleManager::Create(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);
#pragma endregion

#pragma region サウンド
	/*audio = new Audio;
	audio->Initialize();*/
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
	camera->SetTarget({ 0,0,0 });
	camera->SetEye({ 0,2.5f,-7.0f });

}

//void GameScene::Object3dCreate()
//{
//	
//}

void GameScene::Resource2dCreate()
{
	Sprite::LoadTexture(debugTextTexNumber, L"Resource/debugfon.png");
	Sprite::LoadTexture(1, L"Resource/back.png");

	background = Sprite::Create(1, { 0.0f,0.0f });
}

void GameScene::GameInitialize()
{
	player = new Player();
	player->Initialize();
	enemy = new Enemy();
	enemy->Initialize();
	sword = new Sword();
	sword->Initialize(enemy);
	skill = new Skill();
	skill->Initialize();
	cameraAngle = 0;
}

void GameScene::Update()
{
	if (Input::GetInstance()->TriggerKey(DIK_SPACE))
	{
		sceneNo++;

		if (sceneNo > 2)
		{
			sceneNo = 0;
		}

		if (sceneNo == 0)
		{
			GameInitialize();
			camera->SetTarget({ 0,0,0 });
			camera->SetEye({ 0,2.5f,-7.0f });
		}
	}
	//DirectX毎フレーム処理　ここから
	//if (sceneNo == 1)
	{
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

		if (Input::GetInstance()->PushKey(DIK_RIGHT))
		{
			cameraAngle++;
			player->SetAngle(cameraAngle);
		}
		if (Input::GetInstance()->PushKey(DIK_LEFT))
		{
			cameraAngle--;
			player->SetAngle(cameraAngle);
		}
		
		camera->TargetRot({ 0,1.5f,-7.0f }, player->GetPosition(), cameraAngle);
		camera->SetTarget(player->GetPosition());

		camera->Update();
		particleMan->Update();
		light->Update();
		ResourcesUpdate();
		GameUpdate();

		/*if (enemy->GetHP() == 0)
		{
			sceneNo = 3;
		}

		if (player->GetHP() == 0)
		{
			sceneNo = 2;
		}*/

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

		////background->SetColor({ 0,0,0,1 });
		//background->Update();
		
	}
}

void GameScene::ResourcesUpdate()
{
	objScene1->Update();
	objScene3->Update();
	objScene2->Update();
	objScene4->Update();
	objSphere->Update();
	objSphere2->Update();
	object1->Update();
}

void GameScene::GameUpdate()
{
	enemy->Update(player);
	player->Update(camera);
	sword->Update(player, enemy);
	skill->Update(player, enemy);
}

void GameScene::Draw()
{
	dxCommon->PreDraw();
#pragma region 背景スプライト描画
	Sprite::PreDraw(dxCommon->GetCmdList());

	//background->Draw();

	Sprite::PostDraw();
	//深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3D描画
	Object3d::PreDraw(dxCommon->GetCmdList());

	if (sceneNo == 1)
	{
		sword->Draw();
		objScene1->Draw();
		objScene3->Draw();
		objScene2->Draw();
		objScene4->Draw();
		enemy->Draw();
		skill->Draw();
		player->Draw();
	}
	
	
	//objFighter->Draw();
	//objSphere->Draw();
	//objSphere2->Draw();
	object1->Draw(dxCommon->GetCmdList());

	Object3d::PostDraw();

	// パーティクルの描画
	//particleMan->Draw(dxCommon->GetCmdList());
#pragma endregion

#pragma region 前景スプライト描画


	Sprite::PreDraw(dxCommon->GetCmdList());

	/*if (sword->GetIsHit1())
	{
		char str[256];
		sprintf_s(str, "Hit");
		debugText.Print(str, 0, 40, 1.0f);
	}
	if (sword->GetIsHit2())
	{
		char str4[256];
		sprintf_s(str4, "Hit");
		debugText.Print(str4, 0, 55, 1.0f);
	}
	if (sword->GetIsHit3())
	{
		char str5[256];
		sprintf_s(str5, "Hit");
		debugText.Print(str5, 0, 70, 1.0f);
	}

	char str2[256];
	sprintf_s(str2, "x : %f y : %f z : %f", sword->GetCenter().m128_f32[0], sword->GetCenter().m128_f32[1], sword->GetCenter().m128_f32[2]);
	debugText.Print(str2, 0, 10, 1.0f);

	char str3[256];
	sprintf_s(str3, "x : %f y : %f z : %f", sword->GetCenter_enemy().x, sword->GetCenter_enemy().y, sword->GetCenter_enemy().z);
	debugText.Print(str3, 0, 25, 1.0f);*/

	char str6[256];
	sprintf_s(str6, "enemyHP : %d", enemy->GetHP());
	debugText.Print(str6, 0, 10, 1.0f);

	char str5[256];
	sprintf_s(str5, "playerHP : %d", player->GetHP());
	debugText.Print(str5, 0, 30, 1.0f);

	if (sceneNo == 0)
	{
		char str7[256];
		sprintf_s(str7, "Title");
		debugText.Print(str7, WinApp::window_width / 2, WinApp::window_height / 2, 1.0f);
	}

	if (sceneNo == 2)
	{
		char str8[256];
		sprintf_s(str8, "over");
		debugText.Print(str8, WinApp::window_width / 2, WinApp::window_height / 2, 1.0f);
	}

	if (sceneNo == 3)
	{
		char str9[256];
		sprintf_s(str9, "clear");
		debugText.Print(str9, WinApp::window_width / 2, WinApp::window_height / 2, 1.0f);
	}
	
	debugText.DrawAll(dxCommon->GetCmdList());
	Sprite::PostDraw();

	
	dxCommon->PostDraw();
}

void GameScene::Delete()
{
	FbxLoader::GetInstance()->Finalize();
	//DirectX解放
	delete dxCommon;
}
