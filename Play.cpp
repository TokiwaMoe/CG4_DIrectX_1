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

void Play::Initialize(DirectXCommon* dxCommon, Input* keyInput)
{
	assert(dxCommon);
	assert(keyInput);

	this->dxCommon = dxCommon;
	this->input = keyInput;
	playerModel = Object3dModel::LoadFromOBJ("chr_sword");
	player = Object3d::Create();
	player->InitializeGraphicsPipeline(L"Resource/shaders/OBJVertexShader.hlsl", L"Resource/shaders/OBJPixelShader.hlsl");

	player->SetObject3dModel(playerModel);

	player->SetPosition({1,0,0});

	speed = 0.5f;
}

void Play::Update(Camera *camera)
{
#pragma region	移動処理
	if (input->PushKey(DIK_W)) {
		position.z += speed;
		
	}
	if ( input->PushKey(DIK_S)) {
		position.z -= speed;
	}
	if (input->PushKey(DIK_A)) {
		position.x -= speed;
	}
	if (input->PushKey(DIK_D)) {
		position.x += speed;
	}

	player->SetPosition(position);

}

void Play::Draw()
{
	player->Draw();
}