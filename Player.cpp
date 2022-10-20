#include "Player.h"
#include <time.h>
#include <cassert>
#include "SphereCollider.h"
#include "Input.h"

using namespace DirectX;

void Player::Initialize()
{

	playerModel = Object3dModel::LoadFromOBJ("player");
	objPlayer = Object3d::Create();
	objPlayer->InitializeGraphicsPipeline(L"Resource/shaders/OBJVS_Light.hlsl", L"Resource/shaders/OBJPS_Light.hlsl");

	objPlayer->SetObject3dModel(playerModel);
	objPlayer->SetScale({ 0.05,0.05,0.05 });

	playerFbxModel = FbxLoader::GetInstance()->LoadMadelFromFile("player_Run");
	fbxPlayer = new FbxObject3d;
	fbxPlayer->Initialize();
	fbxPlayer->SetModel(playerFbxModel.get());
	fbxPlayer->PlayAnimation();
	fbxPlayer->SetScale({ 0.01,0.01,0.01 });
	fbxPlayer->SetPosition({ 0,0, 5.5 });

	easing = new Easing();
	easing->Initialize();

	time = 0;
	defence_direction = Previous;
	distance = { 0,0,0 };

	otomoAngle = 0;
	HP = 2;
	isKnock = false;
	isWalk = false;
	rote = 0;
}

void Player::Update(Camera *camera)
{
	Move(camera);
	Jump();
	defense();
	knockBack();
	objPlayer->Update();
	fbxPlayer->Update();
}

void Player::Move(Camera* camera)
{
	XMVECTOR speedZ = {0,0,0.1,0};//z
	XMVECTOR speedX = { 0.1,0,0,0 };//x
	XMVECTOR rotY = { 0,0.1,0,0 };//x
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(cameraAngle));//y軸を中心に回転するマトリックスを作成
	speedZ = XMVector3TransformNormal(speedZ, matRot);
	speedX = XMVector3TransformNormal(speedX, matRot);
	rotY = XMVector3TransformNormal(rotY, matRot);
	rotY = XMVector3Normalize(rotY);

	isWalk = false;

	if (Input::GetInstance()->PushKey(DIK_W) || Input::GetInstance()->StickTilt(Input::Stick_Up)) {
		position.z += speedZ.m128_f32[2];
		position.x += speedZ.m128_f32[0];
		//camera->MoveVector({ 0,0,forvardvec.m128_f32[2] });
		rote = 0;
		isWalk = true;
		defence_direction = Previous;

	}
	if (Input::GetInstance()->PushKey(DIK_S) || Input::GetInstance()->StickTilt(Input::Stick_Down)) {
		position.z -= speedZ.m128_f32[2];
		position.x -= speedZ.m128_f32[0];
		//camera->MoveVector({ 0,0,-forvardvec.m128_f32[2] });
		rote = 180;
		isWalk = true;
		defence_direction = Back;
	}
	if (Input::GetInstance()->PushKey(DIK_A) || Input::GetInstance()->StickTilt(Input::Stick_Left)) {
		position.x -= speedX.m128_f32[0];
		position.z -= speedX.m128_f32[2];
		//camera->MoveVector({ -forvardvec.m128_f32[0],0,0 });
		rote = -90;
		isWalk = true;
		defence_direction = Left;
	}
	if (Input::GetInstance()->PushKey(DIK_D) || Input::GetInstance()->StickTilt(Input::Stick_Right)) {
		position.x += speedX.m128_f32[0];
		position.z += speedX.m128_f32[2];
		//camera->MoveVector({ forvardvec.m128_f32[0],0,0 });
		rote = 90;
		isWalk = true;
		defence_direction = Right;
	}

	if (isWalk)
	{
		fbxPlayer->SetRotation({ 0,rote + cameraAngle,0 });
	}
	
	fbxPlayer->SetPosition(position);

}

void Player::defenseKey()
{
	XMVECTOR speedZ = { 0,0,0.1,0 };//z
	XMVECTOR speedX = { 0.1,0,0,0 };//x
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(cameraAngle));//y軸を中心に回転するマトリックスを作成
	speedZ = XMVector3TransformNormal(speedZ, matRot);
	speedX = XMVector3TransformNormal(speedX, matRot);

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
	fbxPlayer->SetPosition(position);
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
	fbxPlayer->SetPosition(position);
}

void Player::knockBack()
{
	XMVECTOR v0_Player = { 0, 0, 0.5f };
	//angleラジアンだけy軸まわりに回転。半径は-100
	XMMATRIX rotM_Player = DirectX::XMMatrixIdentity();
	rotM_Player *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(cameraAngle));
	XMVECTOR v = XMVector3TransformNormal(v0_Player, rotM_Player);
	XMVECTOR playerVec = { position.x, position.y, position.z };
	XMVECTOR playerPos_V = { playerVec.m128_f32[0] - v.m128_f32[0], playerVec.m128_f32[1] + v.m128_f32[1], playerVec.m128_f32[2] - v.m128_f32[2] };
	XMFLOAT3 playerPos = { playerPos_V.m128_f32[0], playerPos_V.m128_f32[1], playerPos_V.m128_f32[2] };

	knock_OldPos = position;
	knock_EndPos = playerPos;
	//isKnock = true;

	if (isKnock)
	{
		knockTime += 0.1f;
		position = easing->ease(knock_OldPos, knock_EndPos, knockTime);

		if (knockTime >= easing->maxflame)
		{
			knockTime = 0;
			isKnock = false;
		}
	}

	objPlayer->SetPosition(position);
	fbxPlayer->SetPosition(position);
}

void Player::Draw(DirectXCommon* dxCommon)
{
	//objPlayer->Draw();
	fbxPlayer->Draw(dxCommon->GetCmdList());
}