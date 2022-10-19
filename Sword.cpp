#include "Sword.h"
#include "Input.h"
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>

void Sword::Initialize(Enemy *enemy)
{
	swordModel = Object3dModel::LoadFromOBJ("sword");
	objSword = Object3d::Create();
	objSword->InitializeGraphicsPipeline(L"Resource/shaders/OBJVS_Light.hlsl", L"Resource/shaders/OBJPS_Light.hlsl");
	objSword->SetObject3dModel(swordModel);
	objSword->SetScale({ 0.02,0.02,0.02 });
	objSword->SetRotation({ 90,0,0 });

	sphereModel = Object3dModel::LoadFromOBJ("sphere");
	for (int i = 0; i < 13; i++)
	{
		objsphere[i] = Object3d::Create();
		objsphere[i]->InitializeGraphicsPipeline(L"Resource/shaders/OBJVS_Light.hlsl", L"Resource/shaders/OBJPS_Light.hlsl");
		//objsphere[i]->SetParent(objSword);
		objsphere[i]->SetObject3dModel(sphereModel);
		objsphere[i]->SetPosition({ 0.5,0.5,0.5 });
		objsphere[i]->SetScale({ swordRadius,swordRadius,swordRadius });
		swordSphere[i].radius = swordRadius;
		
	}

	for (int i = 0; i < 3; i++)
	{
		objsphere_enemy[i] = Object3d::Create();
		objsphere_enemy[i]->InitializeGraphicsPipeline(L"Resource/shaders/OBJVS_Light.hlsl", L"Resource/shaders/OBJPS_Light.hlsl");
		objsphere_enemy[i]->SetObject3dModel(sphereModel);
		objsphere_enemy[i]->SetScale({ enemyRadius,enemyRadius,enemyRadius });
		enemySphere[i].radius = enemyRadius;
	}
}

void Sword::Update(Player* player, Enemy *enemy)
{
	Move(player);
	objSword->Update();
	for (int i = 0; i < 13; i++)
	{
		objsphere[i]->Update();
	}
	
	for (int i = 0; i < 3; i++)
	{
		
		objsphere_enemy[i]->Update();
	}
	
	SwordEnemyCollision(enemy);
}

void Sword::Move(Player* player)
{
	position = player->GetPosition();

	if (Input::GetInstance()->TriggerKey(DIK_4))
	{
		isRote = true;
	}

	if (isRote)
	{
		Angle += 5.0f;
		
		if (Angle >= 90)
		{
			Angle = 0;
			isRote = false;
		}
	}
	
	if (player->defence_direction == player->Previous)
	{
		objSword->SetRotation({ Angle,0,0 });
	}
	else if (player->defence_direction == player->Back)
	{
		objSword->SetRotation({ Angle,180,0 });
	}
	else if (player->defence_direction == player->Left)
	{
		objSword->SetRotation({ Angle,-90,0 });
	}
	else if (player->defence_direction == player->Right)
	{
		objSword->SetRotation({ Angle,90,0 });
	}
	
	objSword->SetPosition(position);
	
}

void Sword::SwordEnemyCollision(Enemy *enemy)
{
	

	for (int i = 0; i < 3; i++)
	{
		XMVECTOR distanse = { 0, 0, enemyRadius + (enemyRadius * (2.0f * i)) };
		//angleƒ‰ƒWƒAƒ“‚¾‚¯yŽ²‚Ü‚í‚è‚É‰ñ“]B”¼Œa‚Í-100
		XMMATRIX rotM_Enemy = DirectX::XMMatrixIdentity();
		rotM_Enemy *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(enemy->Angle));
		XMVECTOR v_enemy = XMVector3TransformNormal(distanse, rotM_Enemy);
		XMVECTOR enemyVec = { enemy->GetPosition().x, enemy->GetPosition().y, enemy->GetPosition().z};
		XMVECTOR enemyPos = { enemyVec.m128_f32[0] + v_enemy.m128_f32[0], enemyVec.m128_f32[1] + v_enemy.m128_f32[1], enemyVec.m128_f32[2] + v_enemy.m128_f32[2] };

		enemySphere[i].center = enemyPos;
		objsphere_enemy[i]->SetPosition({ enemySphere[i].center.m128_f32[0], enemySphere[i].center.m128_f32[1], enemySphere[i].center.m128_f32[2] });
	}
	
	
	for (int i = 0; i < 13; i++)
	{
		XMVECTOR v0_Sword = { 0, swordRadius * 2 * i - 0.5f, 0 };
		//angleƒ‰ƒWƒAƒ“‚¾‚¯yŽ²‚Ü‚í‚è‚É‰ñ“]B”¼Œa‚Í-100
		XMMATRIX rotM_Sword = DirectX::XMMatrixIdentity();
		rotM_Sword *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(Angle));
		XMVECTOR v = XMVector3TransformNormal(v0_Sword, rotM_Sword);
		XMVECTOR swordVec = { position.x, position.y, position.z };
		XMVECTOR swordPos = { swordVec.m128_f32[0] + v.m128_f32[0], swordVec.m128_f32[1] + v.m128_f32[1], swordVec.m128_f32[2] + v.m128_f32[2] };

		swordSphere[i].center = swordPos;
		swordSphere[i].radius = swordRadius;
		objsphere[i]->SetPosition({ swordSphere[i].center.m128_f32[0], swordSphere[i].center.m128_f32[1], swordSphere[i].center.m128_f32[2] });
	}

	

	for (int i = 0; i < 13; i++)
	{
		isHit_enemy1[i] = Collision::CheckSphere2(swordSphere[i], enemySphere[0]);
		isHit_enemy2[i] = Collision::CheckSphere2(swordSphere[i], enemySphere[1]);
		isHit_enemy3[i] = Collision::CheckSphere2(swordSphere[i], enemySphere[2]);
	
		if (isRote)
		{
			if (isHit_enemy1[i] && Decrease == false || isHit_enemy2[i] && Decrease == false || isHit_enemy3[i] && Decrease == false)
			{
				enemy->SetHP(enemy->GetHP() - 1);
				Decrease = true;
			}
		}
		else
		{
			Decrease = false;
		}
		
	}
}

void Sword::Draw()
{
	objSword->Draw();
	/*for (int i = 0; i < 13; i++)
	{
		objsphere[i]->Draw();
	}

	for (int i = 0; i < 3; i++)
	{
		objsphere_enemy[i]->Draw();
	}*/
}

void Sword::SetPosition(XMFLOAT3 pos)
{
	this->position = pos;
}
