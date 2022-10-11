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
		objsphere[i]->SetObject3dModel(sphereModel);
		objsphere[i]->SetScale({0.05,0.05,0.05});
		swordSphere[i].radius = objsphere[i]->GetScale().x;
	}

	for (int i = 0; i < 3; i++)
	{
		objsphere_enemy[i] = Object3d::Create();
		objsphere_enemy[i]->InitializeGraphicsPipeline(L"Resource/shaders/OBJVS_Light.hlsl", L"Resource/shaders/OBJPS_Light.hlsl");
		objsphere_enemy[i]->SetObject3dModel(sphereModel);
		objsphere_enemy[i]->SetScale({ 0.5,0.5,0.5 });
		enemySphere[i].radius = enemyRadius;
	}
}

void Sword::Update(Player* player, Enemy *enemy)
{
	objSword->SetRotation({ 90,0,0 });
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

	if (Input::GetInstance()->PushKey(DIK_4))
	{
		position.x -= 1;
		position.y -= 1;
		objSword->SetRotation({ 90, 0, 30 });
	}
	
	objSword->SetPosition(position);
	
}

void Sword::SwordEnemyCollision(Enemy *enemy)
{
	for (int i = 0; i < 13; i++)
	{
		swordSphere[i].center = {position.x, position.y, position.z - 0.45f + objsphere[i]->GetScale().z * 2 * i};
		swordSphere[i].radius = swordRadius;
		objsphere[i]->SetPosition({ swordSphere[i].center.m128_f32[0], swordSphere[i].center.m128_f32[1], swordSphere[i].center.m128_f32[2]});
	}

	for (int i = 0; i < 3; i++)
	{
		enemySphere[i].center = { enemy->GetPosition().x - 0.95f, enemy->GetPosition().y, enemy->GetPosition().z + 0.5f + (enemyRadius * (2.0f * i)) };
		enemySphere[i].radius = enemyRadius;
		objsphere_enemy[i]->SetPosition({ enemySphere[i].center.m128_f32[0], enemySphere[i].center.m128_f32[1], enemySphere[i].center.m128_f32[2] });
	}

	for (int i = 0; i < 13; i++)
	{
		isHit_enemy1[i] = Collision::CheckSphere2(swordSphere[i], enemySphere[0]);
		isHit_enemy2[i] = Collision::CheckSphere2(swordSphere[i], enemySphere[1]);
		isHit_enemy3[i] = Collision::CheckSphere2(swordSphere[i], enemySphere[2]);
	}

	
}

void Sword::Draw()
{
	objSword->Draw();
	for (int i = 0; i < 13; i++)
	{
		objsphere[i]->Draw();
	}

	for (int i = 0; i < 3; i++)
	{
		objsphere_enemy[i]->Draw();
	}
}

void Sword::SetPosition(XMFLOAT3 pos)
{
	this->position = pos;
}
