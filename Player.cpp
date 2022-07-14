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
	objPlayer = Object3d::Create();
	objPlayer->InitializeGraphicsPipeline(L"Resource/shaders/OBJVS_Light.hlsl", L"Resource/shaders/OBJPS_Light.hlsl");

	objPlayer->SetObject3dModel(playerModel);
	objPlayer->SetScale({ 0.5,0.5,0.5 });
	
}

void Player::Update(Camera *camera)
{
	Move(camera);
	Jump();
	objPlayer->Update();
}

void Player::Move(Camera* camera)
{
	XMVECTOR forvardvec = {0.1,0.1,0.1,0.1};
	if (Input::GetInstance()->PushKey(DIK_W)) {
		position.z += forvardvec.m128_f32[2];
		objPlayer->SetRotation({ 0,0,0 });
	}
	if (Input::GetInstance()->PushKey(DIK_S)) {
		position.z -= forvardvec.m128_f32[2];
		objPlayer->SetRotation({ 0,180,0 });
	}
	if (Input::GetInstance()->PushKey(DIK_A)) {
		position.x -= forvardvec.m128_f32[0];
		objPlayer->SetRotation({ 0,90,0 });
	}
	if (Input::GetInstance()->PushKey(DIK_D)) {
		position.x += forvardvec.m128_f32[0];
		objPlayer->SetRotation({ 0,-90,0 });
	}

	objPlayer->SetPosition(position);
}

void Player::defense()
{
	if (Input::GetInstance()->TriggerKey(DIK_1))
	{
		defenceFlag = true;
	}
}

void Player::Jump()
{

	float dy = speed * time;
	float dx = speed * time;
	float g = k * speed / m;
	float vx = dx * cos((PI / 180) * angle);
	float vy = dy * sin((PI / 180) * angle) - 0.5 * (g * (time * time));
	float vy2 = dy * sin((PI / 180) * angle) - 0.5 * (gravity * (time * time));

	time += 0.1;

	if (Input::GetInstance()->TriggerKey(DIK_2) && jumpFlag == false)
	{
		position.x = vx;
		position.y = vy;
	}

	if (jumpFlag == true)
	{
		
	}/*

	if (gravityFlag == true)
	{
		jumpFlag = false;
		position.y -= gravity / 60.0f;

		if (position.y <= 0)
		{
			gravityFlag = false;
		}
	}*/

	objPlayer->SetPosition(position);
}

void Player::Draw()
{
	objPlayer->Draw();
}