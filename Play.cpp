#include "Play.h"
#include "Camera.h"
#include <time.h>
#include <cassert>
#include "SphereCollider.h"

using namespace DirectX;

Play::Play()
{
	
	
}

Play::~Play()
{


}

void Play::Initialize(Input* keyInput)
{
	this->input = keyInput;

	playerModel = Object3dModel::LoadFromOBJ("chr_sword");
	player = Object3d::Create();
	player->InitializeGraphicsPipeline(L"Resource/shaders/OBJVertexShader.hlsl", L"Resource/shaders/OBJPixelShader.hlsl");

	player->SetObject3dModel(playerModel);

	speed = 0.5f;
	player->SetPosition({ 1,0,0 });
}

void Play::Update(Camera *camera)
{
	Move();
	player->Update();
}

void Play::Move()
{
	if (input->PushKey(DIK_W)) {
		position.z += speed;
		//camera->Move({ 0,0,-speed });
	}
	if (input->PushKey(DIK_S)) {
		position.z -= speed;
		//camera->Move({ 0,0,speed });
	}
	if (input->PushKey(DIK_A)) {
		position.x -= speed;
		//camera->Move({ speed,0,0 });
	}
	if (input->PushKey(DIK_D)) {
		position.x += speed;
		//camera->Move({ -speed,0,0 });
	}

	player->SetPosition(position);
}

void Play::Draw()
{
	player->Draw();
}