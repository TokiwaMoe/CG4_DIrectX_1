#include "Sword.h"
#include "Input.h"
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>

using namespace DirectX;

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
		objsphere[i]->SetScale({0.05,0.05,0.05});
		swordSphere[i].radius = objsphere[i]->GetScale().x;
		
	}

	for (int i = 0; i < 3; i++)
	{
		objsphere_enemy[i] = Object3d::Create();
		objsphere_enemy[i]->InitializeGraphicsPipeline(L"Resource/shaders/OBJVS_Light.hlsl", L"Resource/shaders/OBJPS_Light.hlsl");
		objsphere_enemy[i]->SetObject3dModel(sphereModel);
		objsphere_enemy[i]->SetParent(enemy->objEnemy);
		objsphere_enemy[i]->SetScale({ enemyRadius,enemyRadius,enemyRadius });
		enemySphere[i].radius = enemyRadius;
		objsphere_enemy[i]->SetPosition(
			{ enemy->GetPosition().x - enemyRadius,
			  enemy->GetPosition().y,
			enemy->GetPosition().z + enemyRadius + (enemyRadius * (2.0f * i)) }
		);

	}
}

void Sword::Update(Player* player, Enemy *enemy)
{
	Move(player);
	objSword->Update();

	enemyOBB.m_Pos = { enemy->GetPosition().x, enemy->GetPosition().y, enemy->GetPosition().z + 1.5f };
	for (int i = 0; i < 3; i++)
	{
		XMMATRIX enemy_vector = XMMatrixIdentity();
		enemy_vector *= XMMatrixRotationY(XMConvertToRadians(enemy->objEnemy->GetRotation().y));
		enemyOBB.m_NormaDirect[i]={ enemy_vector.r->m128_f32[0], enemy_vector.r->m128_f32[1], enemy_vector.r->m128_f32[2] };

		enemyOBB.m_fLength[0] = 1.0f;
		enemyOBB.m_fLength[1] = 1.0f;
		enemyOBB.m_fLength[2] = 1.5f;
	}

	isHit = Collision::ColOBBs(swordOBB, enemyOBB);

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
	position = { player->GetPosition().x, player->GetPosition().y, player->GetPosition().z };
	XMVECTOR v = { 0,0,0,0 };
	XMVECTOR sword_vector = { 0,0.1,0,0 };//x
	XMMATRIX matRot_Sword = XMMatrixIdentity();
	matRot_Sword *= XMMatrixRotationX(XMConvertToRadians(Angle));//y軸を中心に回転するマトリックスを作成
	v = XMVector3TransformNormal(sword_vector, matRot_Sword);
	XMVECTOR v3 = { 0,0,0,0 };

	if (Input::GetInstance()->TriggerKey(DIK_4))
	{
		isRote = true;
	}

	if (isRote)
	{
		Angle += 5.0f;
		v3 = v + swordOBB.m_Pos;
		if (Angle >= 90)
		{
			Angle = 0;
			isRote = false;
		}
	}
	else
	{
		v3 = { position.x, position.y, position.z };
	}

	for (int i = 0; i < 3; i++)
	{
		//swordOBB.m_Pos = { position.x, position.y, position.z };
		swordOBB.m_Pos = v3;
		swordOBB.m_NormaDirect[i] = v + swordOBB.m_Pos;

		swordOBB.m_fLength[0] = 0.5f;
		swordOBB.m_fLength[1] = 1.0f;
		swordOBB.m_fLength[2] = 2.0f;
	}
	
	objSword->SetRotation({ Angle,0,0 });
	objSword->SetPosition(position);
	
}

void Sword::SwordEnemyCollision(Enemy *enemy)
{
	for (int i = 0; i < 3; i++)
	{
		pos[i].x = objsphere_enemy[i]->GetPosition().x;
		pos[i].y = objsphere_enemy[i]->GetPosition().y;
		pos[i].z = objsphere_enemy[i]->GetPosition().z;

		//objsphere_enemy[i]->SetPosition(pos[i]);
		/*enemySphere[i].center = {
		objsphere_enemy[i]->GetPosition().x,
		objsphere_enemy[i]->GetPosition().y,
		objsphere_enemy[i]->GetPosition().z
		};*/
	}
	
	
	for (int i = 0; i < 13; i++)
	{
		
		swordSphere[i].center = {position.x, position.y - 0.5f + swordRadius * 2 * i, position.z};
		swordSphere[i].radius = swordRadius;
		objsphere[i]->SetPosition({ swordSphere[i].center.m128_f32[0], swordSphere[i].center.m128_f32[1], swordSphere[i].center.m128_f32[2] });
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
