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
	

	objFriend->SetObject3dModel(friendModel);
	objFriend->SetScale({ 0.1,0.1,0.1 });

	for (int i = 0; i < 5; i++)
	{
		objBullet[i] = Object3d::Create();
		objBullet[i]->InitializeGraphicsPipeline(L"Resource/shaders/OBJVS_Light.hlsl", L"Resource/shaders/OBJPS_Light.hlsl");
		objBullet[i]->SetObject3dModel(bulletModel);
		objBullet[i]->SetScale({0.1,0.1,0.1});

		bulletFlag[i] = false;
	}

	easing = new Easing();
	easing->Initialize();
}

void Skill::Update(Player* player, Enemy *enemy)
{
	Move(player);
	HomingBullet(player, enemy);
	//bulletEnemyCollision(enemy);
	objFriend->Update();

	for (int i = 0; i < 5; i++)
	{
		objBullet[i]->Update();
	}
	
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
	for (int i = 0; i < 5; i++)
	{
		if (Input::GetInstance()->TriggerKey(DIK_3) && bulletFlag[i] == false)
		{
			bulletPos[i] = position;
			enemyOldPos = enemy->GetPosition();
			bulletTime[i] = 0;
			bulletFlag[i] = true;
			for (int i = 0; i < 5; i++)
			{
				point[i].p0 = bulletPos[i];
				point[i].p1 = { point[i].p0.x - 1.0f + 0.5f * i, point[i].p0.y + 2.0f, point[i].p0.z - 0.5f };
				point[i].p2 = { point[i].p1.x - 1.0f + 0.5f * i, point[i].p1.y, point[i].p1.z - 0.5f };
				point[i].p3 = enemyOldPos;
			}

		}

		if (bulletFlag[i])
		{
			bulletTime[i] += 0.05f;
			a[i] = easing->lerp(point[i].p0, point[i].p1, bulletTime[i]);
			b[i] = easing->lerp(point[i].p1, point[i].p2, bulletTime[i]);
			c[i] = easing->lerp(point[i].p2, point[i].p3, bulletTime[i]);
			d[i] = easing->lerp(a[i], b[i], bulletTime[i]);
			e[i] = easing->lerp(b[i], c[i], bulletTime[i]);
			bulletPos[i] = easing->lerp(d[i], e[i], bulletTime[i]);
			if (bulletTime[i] >= 1.0f)
			{
				bulletFlag[i] = false;

			}
		}

		objBullet[i]->SetPosition(bulletPos[i]);
	}
	
}

void Skill::Draw()
{
	objFriend->Draw();
	for (int i = 0; i < 5; i++)
	{
		objBullet[i]->Draw();
	}
	
}

void Skill::bulletEnemyCollision(Enemy* enemy)
{
	XMVECTOR distanse = { 0, 0, enemyRad };
	//angleƒ‰ƒWƒAƒ“‚¾‚¯yŽ²‚Ü‚í‚è‚É‰ñ“]B”¼Œa‚Í-100
	XMMATRIX rotM_Enemy = DirectX::XMMatrixIdentity();
	rotM_Enemy *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(enemy->Angle));
	XMVECTOR v_enemy = XMVector3TransformNormal(distanse, rotM_Enemy);
	XMVECTOR enemyVec = { enemy->GetPosition().x, enemy->GetPosition().y, enemy->GetPosition().z };
	XMVECTOR enemyPos = { enemyVec.m128_f32[0] + v_enemy.m128_f32[0], enemyVec.m128_f32[1] + v_enemy.m128_f32[1], enemyVec.m128_f32[2] + v_enemy.m128_f32[2] };

	enemySp.center = enemyPos;

	for (int i = 0; i < 5; i++)
	{
		bulletSp[i].center = { bulletPos[i].x, bulletPos[i].y, bulletPos[i].z };
		bulletSp[i].radius = bulletRad;

		if (Collision::CheckSphere2(bulletSp[i], enemySp) && isHit == false)
		{
			enemy->SetHP(enemy->GetHP() - 1);
			isHit = true;
		}
		else {
			isHit = false;
		}
	}


	//objsphere_enemy[i]->SetPosition({ enemySphere[i].center.m128_f32[0], enemySphere[i].center.m128_f32[1], enemySphere[i].center.m128_f32[2] });
}
