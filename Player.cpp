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

	easing = new Easing();
	easing->Initialize();
}

void Player::Update(Camera *camera)
{
	Move(camera);
	Jump();
	defense();
	objPlayer->Update();
}

void Player::Move(Camera* camera)
{
	XMVECTOR forvardvec = {0.1,0.1,0.1,0.1};
	if (Input::GetInstance()->PushKey(DIK_W)) {
		position.z += forvardvec.m128_f32[2];
		objPlayer->SetRotation({ 0,0,0 });
		defence_direction = Previous;
	}
	if (Input::GetInstance()->PushKey(DIK_S)) {
		position.z -= forvardvec.m128_f32[2];
		objPlayer->SetRotation({ 0,180,0 });
		defence_direction = Back;
	}
	if (Input::GetInstance()->PushKey(DIK_A)) {
		position.x -= forvardvec.m128_f32[0];
		objPlayer->SetRotation({ 0,-90,0 });
		defence_direction = Left;
	}
	if (Input::GetInstance()->PushKey(DIK_D)) {
		position.x += forvardvec.m128_f32[0];
		objPlayer->SetRotation({ 0,90,0 });
		defence_direction = Right;
	}

	objPlayer->SetPosition(position);
}

void Player::defense()
{
	float time = 0;
	if (Input::GetInstance()->TriggerKey(DIK_1))
	{
		defenceFlag = true;
	}

	XMFLOAT3 oldPos = objPlayer->GetPosition();
	if (defenceFlag)
	{
		if (defence_direction == Previous)
		{
			time += 0.1f;
			position = easing->easeOut(oldPos, { oldPos.x, oldPos.y, oldPos.z + 10.0f }, time);

			if (time >= easing->maxflame)
			{
				defenceFlag = false;
			}
		}
	}
	

	objPlayer->SetPosition(position);
}

void Player::Jump()
{

	if (Input::GetInstance()->TriggerKey(DIK_2) && jumpFlag == false)
	{
		jumpFlag = true;
	}

	if (jumpFlag == true)
	{
		position.y += 0.5f;
		if (position.y >= 4)
		{
			gravityFlag = true;
		}
	}

	if (gravityFlag == true)
	{
		jumpFlag = false;
		position.y -= gravity / 60.0f;

		if (position.y < 0.5)
		{
			gravityFlag = false;
		}
	}

	objPlayer->SetPosition(position);
}

void Player::Draw()
{
	objPlayer->Draw();
}