#include "Skill.h"
#include"Input.h"
#include "SphereCollider.h"
#include <time.h>
#include <cassert>

using namespace DirectX;

void Skill::Initialize()
{
	friendModel = Object3dModel::LoadFromOBJ("sphere");
	objFriend = Object3d::Create();
	objFriend->InitializeGraphicsPipeline(L"Resource/shaders/OBJVS_Light.hlsl", L"Resource/shaders/OBJPS_Light.hlsl");
	bulletModel = Object3dModel::LoadFromOBJ("sphere");
	objBullet = Object3d::Create();
	objBullet->InitializeGraphicsPipeline(L"Resource/shaders/OBJVS_Light.hlsl", L"Resource/shaders/OBJPS_Light.hlsl");

	objFriend->SetObject3dModel(friendModel);
	objFriend->SetScale({ 0.1,0.1,0.1 });
	objBullet->SetObject3dModel(bulletModel);
	objBullet->SetScale({ 0.1,0.1,0.1 });

	easing = new Easing();
	easing->Initialize();
}

void Skill::Update(Player* player)
{
	Move(player);
	HomingBullet(player);
	objFriend->Update();
	objBullet->Update();
}

void Skill::Move(Player* player)
{
	float limitSpeed = 0.3f;     //弾の制限速度
	XMFLOAT3 playerPos = player->GetPosition();
	lengthVec = { playerPos.x - position.x, playerPos.y - position.y, playerPos.z - position.z };  //弾から追いかける対象への方向を計算
	XMVector3Normalize(lengthVec);
	lengthVec = { (lengthVec.m128_f32[0] - 1.0f) * bulletSpeed, (lengthVec.m128_f32[1] + 1.0f) * bulletSpeed, lengthVec.m128_f32[2] * bulletSpeed };  //方向の長さを1に正規化、任意の力をAddForceで加える

	float speedXTemp = min(max(lengthVec.m128_f32[0], -limitSpeed), limitSpeed); //X方向の速度を制限
	float speedYTemp = min(max(lengthVec.m128_f32[1], -limitSpeed), limitSpeed);  //Y方向の速度を制限
	float speedZTemp = min(max(lengthVec.m128_f32[2], -limitSpeed), limitSpeed);

	position.x += speedXTemp;
	position.y += speedYTemp;
	position.z += speedZTemp;

	objFriend->SetPosition(position);
}

void Skill::HomingBullet(Player* player)
{
	bulletPos = player->GetPosition();
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

void Skill::Draw()
{
	objFriend->Draw();
	objBullet->Draw();
}
