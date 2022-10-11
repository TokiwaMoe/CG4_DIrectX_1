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
	
	
}

void Sword::Update(Player* player, Enemy *enemy)
{
	objSword->SetRotation({ 90,0,0 });
	Move(player);
	objSword->Update();

	swordOBB.m_Pos = { position.x, position.y, position.z };
	for (int i = 0; i < 3; i++)
	{
		XMMATRIX sword_vector;
		sword_vector = DirectX::XMMatrixRotationRollPitchYaw(objSword->GetRotation().y, objSword->GetRotation().x, objSword->GetRotation().z);

		swordOBB.m_NormaDirect[i] = { sword_vector.r->m128_f32[0], sword_vector.r->m128_f32[1], sword_vector.r->m128_f32[2] };

		swordOBB.m_fLength[0] = 0.5f;
		swordOBB.m_fLength[1] = 1.0f;
		swordOBB.m_fLength[2] = 2.0f;
	}

	enemyOBB.m_Pos = { enemy->GetPosition().x, enemy->GetPosition().y, enemy->GetPosition().z + 1.5f };
	for (int i = 0; i < 3; i++)
	{
		XMMATRIX enemy_vector;
		enemy_vector = DirectX::XMMatrixRotationRollPitchYaw(enemy->objEnemy->GetRotation().y, enemy->objEnemy->GetRotation().x, enemy->objEnemy->GetRotation().z);
		enemyOBB.m_NormaDirect[i] = { 0,0,0 };

		enemyOBB.m_fLength[0] = 1.0f;
		enemyOBB.m_fLength[1] = 1.0f;
		enemyOBB.m_fLength[2] = 1.5f;
	}
	isHit = Collision::ColOBBs(swordOBB, enemyOBB);
	
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

void Sword::Draw()
{
	objSword->Draw();
}

void Sword::SetPosition(XMFLOAT3 pos)
{
	this->position = pos;
}
