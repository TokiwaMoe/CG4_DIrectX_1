#include "Player.h"
#include "Camera.h"
#include <time.h>
#include <cassert>
#include "SphereCollider.h"
#include "Input.h"

using namespace DirectX;

Player::Player()
{
}

Player::~Player()
{
}

void Player::Initialize()
{

	playerModel = Object3dModel::LoadFromOBJ("chr_sword");
	player = Object3d::Create();
	player->InitializeGraphicsPipeline(L"Resource/shaders/OBJVS_Light.hlsl", L"Resource/shaders/OBJPS_Light.hlsl");

	player->SetObject3dModel(playerModel);
	player->SetScale({ 0.5,0.5,0.5 });

	speed = 0.5f;
	
}

void Player::Update(Camera *camera)
{
	Move(camera);
	player->Update();
}

void Player::Move(Camera* camera)
{
	XMVECTOR forvardvec = {1,1,1,1};
	if (Input::GetInstance()->PushKey(DIK_W)) {
		position.z += forvardvec.m128_f32[2];
	}
	if (Input::GetInstance()->PushKey(DIK_S)) {
		position.z -= forvardvec.m128_f32[2];
	}
	if (Input::GetInstance()->PushKey(DIK_A)) {
		position.x -= forvardvec.m128_f32[0];
	}
	if (Input::GetInstance()->PushKey(DIK_D)) {
		position.x += forvardvec.m128_f32[0];
	}

	player->SetPosition(position);
}

void Player::Draw()
{
	player->Draw();
}