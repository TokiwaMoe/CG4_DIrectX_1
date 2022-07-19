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

	time = 0;
	defence_direction = Previous;
	distance = { 0,0,0 };
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
	XMVECTOR forvardvec = {0.5,0.5,0.5,0.5};
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
	if (Input::GetInstance()->TriggerKey(DIK_1) && defenceFlag == false)
	{
		time = 0;
		distance = { 0,0,0 };
		defenceFlag = true;
		oldPos = objPlayer->GetPosition();
	}

	
	if (defenceFlag)
	{
		defenseKey();
		
	}
	

	objPlayer->SetPosition(position);
}

void Player::defenseKey()
{
	/*if (Input::GetInstance()->PushKey(DIK_W)) {
		distance = { 0,0,5 };
	}
	if (Input::GetInstance()->PushKey(DIK_S)) {
		distance = { 0,0,-5 };
	}
	if (Input::GetInstance()->PushKey(DIK_A)) {
		distance = { -5,0,0 };
	}
	if (Input::GetInstance()->PushKey(DIK_D)) {
		distance = { 5,0,0 };
	}*/
	if (defence_direction == Previous)
	{
		distance = { 0,0,5 };
	}
	else if (defence_direction == Back)
	{
		distance = { 0,0,-5 };
	}
	else if (defence_direction == Left)
	{
		distance = { -5,0,0 };
	}
	else if (defence_direction == Right)
	{
		distance = { 5,0,0 };
	}
	endPos = { oldPos.x + distance.x, oldPos.y + distance.y, oldPos.z + distance.z };
	defenseMove(endPos);
}

void Player::defenseMove(XMFLOAT3 FinalPos)
{
	if (time >= 0 && time <= easing->maxflame)
	{
		time += 0.1f;
		position = easing->ease(oldPos, FinalPos, time);
	}

	if (time >= easing->maxflame)
	{
		distance = { 0,0,0 };
		storagePos = position;
		defenceFlag = false;
	}
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