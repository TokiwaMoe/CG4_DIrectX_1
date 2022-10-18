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

void Skill::Update(Player* player, Enemy *enemy)
{
	Move(player);
	HomingBullet(player, enemy);
	objFriend->Update();
	objBullet->Update();
}

void Skill::Move(Player* player)
{
	float limitSpeed = 0.3f;     //’e‚Ì§ŒÀ‘¬“x
	XMFLOAT3 playerPos = player->GetPosition();
	lengthVec = { playerPos.x - position.x, playerPos.y - position.y, playerPos.z - position.z };  //’e‚©‚ç’Ç‚¢‚©‚¯‚é‘ÎÛ‚Ö‚Ì•ûŒü‚ðŒvŽZ
	XMVector3Normalize(lengthVec);
	lengthVec = { (lengthVec.m128_f32[0] - 1.0f) * bulletSpeed, (lengthVec.m128_f32[1] + 1.0f) * bulletSpeed, lengthVec.m128_f32[2] * bulletSpeed };  //•ûŒü‚Ì’·‚³‚ð1‚É³‹K‰»A”CˆÓ‚Ì—Í‚ðAddForce‚Å‰Á‚¦‚é

	float speedXTemp = min(max(lengthVec.m128_f32[0], -limitSpeed), limitSpeed); //X•ûŒü‚Ì‘¬“x‚ð§ŒÀ
	float speedYTemp = min(max(lengthVec.m128_f32[1], -limitSpeed), limitSpeed);  //Y•ûŒü‚Ì‘¬“x‚ð§ŒÀ
	float speedZTemp = min(max(lengthVec.m128_f32[2], -limitSpeed), limitSpeed);

	position.x += speedXTemp;
	position.y += speedYTemp;
	position.z += speedZTemp;

	objFriend->SetPosition(position);
}

void Skill::HomingBullet(Player* player, Enemy *enemy)
{
	
	if (Input::GetInstance()->TriggerKey(DIK_3) && bulletFlag == false)
	{
		bulletPos = position;
		enemyOldPos = enemy->GetPosition();
		bulletTime = 0;
		bulletFlag = true;
		point.p0 = bulletPos;
		point.p1 = { point.p0.x + 1.0f, point.p0.y + 2.0f, point.p0.z - 0.5f };
		point.p2 = { point.p1.x + 1.0f, point.p1.y, point.p1.z - 0.5f };
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
			
		}
	}
	else
	{
		
	}

	objBullet->SetPosition(bulletPos);
}

void Skill::Draw()
{
	objFriend->Draw();
	objBullet->Draw();
}
