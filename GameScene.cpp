#include "GameScene.h"


void GameScene::Initialize(WinApp* winApp)
{
	this->dxCommon = dxCommon;
	this->input = input;
	this->audio = audio;

	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

#pragma region �L�[�擾
	input = Input::GetInstance();
	input->Initialize(winApp);
	input->MouseInitialize(winApp);
	input->PadInitialize(winApp);
#pragma endregion

#pragma region �J����
	// �J��������
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height, input);
	// 3D�I�u�W�F�N�g�ɃJ�������Z�b�g
	Object3d::SetCamera(camera);
#pragma endregion

#pragma region Object3d
	Object3d::StaticInitialize(dxCommon->GetDev(), camera);
#pragma endregion

#pragma region FBX
	//�f�o�C�X���Z�b�g
	FbxObject3d::SetDevice(dxCommon->GetDev());
	//�J�������Z�b�g
	FbxObject3d::SetCamera(camera);
	//�O���t�B�b�N�p�C�v���C������
	FbxObject3d::CreateGraphicsPipline();
	//FBX
	FbxLoader::GetInstance()->Initiallize(dxCommon->GetDev());
#pragma endregion

#pragma region �X�v���C�g
	Sprite::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);

	debugText.Initialize(debugTextTexNumber);
#pragma endregion

#pragma region �p�[�e�B�N��
	particleMan = ParticleManager::Create(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);
#pragma endregion

#pragma region �T�E���h
	audio = new Audio;
	audio->Initialize();
#pragma endregion

#pragma region ���C�g
	Light::StaticInitialize(dxCommon->GetDev());
	//���C�g����
	light = Light::Create();
	//���C�g�F��ݒ�
	light->SetLightColor({ 1,1,1 });
	//3D�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	Object3d::SetLight(light);
#pragma endregion

	// �J���������_���Z�b�g
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

	//���f�������w�肵�ăt�@�C���ǂݍ���
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
	//DirectX���t���[�������@��������
	input->KeyUpdate();
	input->MouseUpdate();
	//�}�E�X���W
	POINT mousePos;
	GetCursorPos(&mousePos);
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
#pragma region �w�i�X�v���C�g�`��
	Sprite::PreDraw(dxCommon->GetCmdList());

	//background->Draw();

	Sprite::PostDraw();
	//�[�x�o�b�t�@�N���A
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3D�`��
	Object3d::PreDraw(dxCommon->GetCmdList());

	/*objSkydome->Draw();
	objGround->Draw();
	objFighter->Draw();
	objSphere->Draw();
	objSphere2->Draw();*/
	//object1->Draw(dxCommon->GetCmdList());

	Object3d::PostDraw();

	// �p�[�e�B�N���̕`��
	particleMan->Draw(dxCommon->GetCmdList());
#pragma endregion

#pragma region �O�i�X�v���C�g�`��


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
	//DirectX���
	delete dxCommon;
}
