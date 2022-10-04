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
		XMMATRIX rotM = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(90.0f));
		XMVECTOR vector = XMVector3TransformNormal({0,0,0}, rotM);
		swordOBB.m_NormaDirect[i] = { objSword->GetRotation().x, 0, 0};
		
		swordOBB.m_fLength[0] = 0.5f;
		swordOBB.m_fLength[1] = 0.5f;
		swordOBB.m_fLength[2] = 0.5f;
	}

	enemyOBB.m_Pos = { enemy->GetPosition().x - 1.0f, enemy->GetPosition().y, enemy->GetPosition().z };
	for (int i = 0; i < 3; i++)
	{
		enemyOBB.m_NormaDirect[i] = { 0, 0, 0 };

		enemyOBB.m_fLength[0] = 0.5f;
		enemyOBB.m_fLength[1] = 0.5f;
		enemyOBB.m_fLength[2] = 3.0f;
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
