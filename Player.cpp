#include "Player.h"
#include <time.h>
#include <cassert>
#include "SphereCollider.h"
#include "Input.h"

using namespace DirectX;

void Player::Initialize()
{

	playerModel = Object3dModel::LoadFromOBJ("chr_sword");
	objPlayer = Object3d::Create();
	objPlayer->InitializeGraphicsPipeline(L"Resource/shaders/OBJVS_Light.hlsl", L"Resource/shaders/OBJPS_Light.hlsl");
	bulletModel = Object3dModel::LoadFromOBJ("sphere");
	objBullet = Object3d::Create();
	objBullet->InitializeGraphicsPipeline(L"Resource/shaders/OBJVS_Light.hlsl", L"Resource/shaders/OBJPS_Light.hlsl");
	friendModel = Object3dModel::LoadFromOBJ("sphere");
	objFriend = Object3d::Create();
	objFriend->InitializeGraphicsPipeline(L"Resource/shaders/OBJVS_Light.hlsl", L"Resource/shaders/OBJPS_Light.hlsl");

	objPlayer->SetObject3dModel(playerModel);
	objPlayer->SetScale({ 0.5,0.5,0.5 });
	objBullet->SetObject3dModel(bulletModel);
	objBullet->SetScale({ 0.1,0.1,0.1 });
	objFriend->SetObject3dModel(friendModel);
	objFriend->SetScale({ 0.1,0.1,0.1 });

	easing = new Easing();
	easing->Initialize();

	time = 0;
	defence_direction = Previous;
	distance = { 0,0,0 };

	otomoAngle = 0;
}

void Player::Update(Camera *camera)
{
	Move(camera);
	Jump();
	defense();
	HomingBullet();
	OtomoMove();
	objPlayer->Update();
	objBullet->Update();
	objFriend->Update();
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
		position.y += gravity / 60.0f;
		if (position.y >= 4)
		{
			gravityFlag = true;
		}
	}

	if (gravityFlag == true)
	{
		jumpFlag = false;
		position.y -= gravity / 60.0f;

		if (position.y < 0)
		{
			gravityFlag = false;
		}
	}

	objPlayer->SetPosition(position);
}

void Player::HomingBullet()
{
	
	bulletPos = position;
	if (Input::GetInstance()->TriggerKey(DIK_3) && bulletFlag == false)
	{
		//bulletTime = 0;
		enemyOldPos = { 0,0,0 };
		
		bulletFlag = true;
		point.p0 = bulletPos;
		point.p1 = { point.p0.x + 0.5f, point.p0.y + 2.0f, point.p0.z - 0.5f };
		point.p2 = { point.p1.x + 0.5f, point.p1.y, point.p1.z - 0.5f };
		point.p3 = enemyOldPos;
	}

	if (bulletFlag)
	{
		bulletTime += 0.05f;
		a = easing->lerp(point.p0, point.p1, bulletTime);
		b = easing->lerp(point.p1, point.p2, bulletTime);
		c = easing->lerp(point.p2, point.p3, bulletTime);
		d = easing->lerp(a, b, bulletTime);
		e = easing->lerp(b, c, bulletTime);
		bulletPos = easing->lerp(d, e, bulletTime);
		if (bulletTime >= 1.0f)
		{
			bulletFlag = false;
			bulletTime = 0;
		}
	}

	objBullet->SetPosition(bulletPos);
}

void Player::OtomoMove()
{
	float limitSpeed = 0.3f;     //弾の制限速度

	lengthVec = { position.x - friendPos.x, position.y - friendPos.y, position.z - friendPos.z };  //弾から追いかける対象への方向を計算
	XMVector3Normalize(lengthVec);
	lengthVec = { (lengthVec.m128_f32[0] - 1.0f) * bulletSpeed, (lengthVec.m128_f32[1] + 1.0f) * bulletSpeed, lengthVec.m128_f32[2] * bulletSpeed };  //方向の長さを1に正規化、任意の力をAddForceで加える

	float speedXTemp = min(max(lengthVec.m128_f32[0], -limitSpeed), limitSpeed); //X方向の速度を制限
	float speedYTemp = min(max(lengthVec.m128_f32[1], -limitSpeed), limitSpeed);  //Y方向の速度を制限
	float speedZTemp = min(max(lengthVec.m128_f32[2], -limitSpeed), limitSpeed);

	friendPos.x += speedXTemp;
	friendPos.y += speedYTemp;
	friendPos.z += speedZTemp;

	objFriend->SetPosition(friendPos);
}

void Player::Draw()
{
	objPlayer->Draw();
	objBullet->Draw();
	objFriend->Draw();
}