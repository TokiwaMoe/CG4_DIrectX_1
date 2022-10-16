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
	XMVECTOR sword_vectorX = { 1,0,0,0 };//x
	XMVECTOR sword_vectorZ = { 0,0,1,0 };//z
	XMMATRIX matRot_Sword = XMMatrixIdentity();
	matRot_Sword *= XMMatrixRotationX(XMConvertToRadians(objSword->GetRotation().x));//y軸を中心に回転するマトリックスを作成
	matRot_Sword *= XMMatrixRotationZ(XMConvertToRadians(objSword->GetRotation().z));//y軸を中心に回転するマトリックスを作成
	sword_vectorX = XMVector3TransformNormal(sword_vectorX, matRot_Sword);
	sword_vectorZ = XMVector3TransformNormal(sword_vectorZ, matRot_Sword);

	for (int i = 0; i < 3; i++)
	{
		swordOBB.m_Pos = { position.x, position.y, position.z };

		swordOBB.m_NormaDirect[i] = { matRot_Sword.r->m128_f32[0], matRot_Sword.r->m128_f32[1], matRot_Sword.r->m128_f32[2] };

		swordOBB.m_fLength[0] = 0.5f;
		swordOBB.m_fLength[1] = 1.0f;
		swordOBB.m_fLength[2] = 2.0f;
	}

	if (Input::GetInstance()->PushKey(DIK_4))
	{
		position.x -= 1;
		position.y -= 1;
		swordOBB.m_Pos.x -= sword_vectorX.m128
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
