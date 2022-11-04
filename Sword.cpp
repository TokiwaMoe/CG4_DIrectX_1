#include "Sword.h"
#include "Input.h"
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>

void Sword::Initialize(Enemy *enemy, DirectXCommon* dxCommon, Camera* camera)
{
	swordModel = Object3dModel::LoadFromOBJ("sword");
	objSword = Object3d::Create();
	objSword->InitializeGraphicsPipeline(L"Resource/shaders/OBJVS_Light.hlsl", L"Resource/shaders/OBJPS_Light.hlsl");
	objSword->SetObject3dModel(swordModel);
	objSword->SetScale({ 0.01,0.01,0.01 });
	objSword->SetRotation({ 0,0,0 });

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

	effects = new Effects();
	effects->Initialize(dxCommon->GetDev(), dxCommon->GetCmdQueue(), camera);
}

void Sword::Update(Player* player, Enemy *enemy, DirectXCommon* dxCommon, Camera* camera)
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
	
	SwordEnemyCollision(enemy, dxCommon, camera);
}

void Sword::Move(Player* player)
{
	// 剣が参照するボーン座標系での座標
	XMVECTOR offset = { 0,0.5f,0,1 };
	// ボーンの計算済みワールド行列（ボーンのローカル行列×プレイヤーのワールド行列）
	XMMATRIX boneWorldMatrix = player->GetTransform() * player->GetMatWorld();
	// ボーンローカル座標からワールド座標に変換
	offset = DirectX::XMVector3Transform(offset, boneWorldMatrix);

	position = {
		boneWorldMatrix.r[3].m128_f32[0],
		boneWorldMatrix.r[3].m128_f32[1],
		boneWorldMatrix.r[3].m128_f32[2]
	};

	rotation = { player->fbxPlayer_Attack->GetRotation().x,
				 player->fbxPlayer_Attack->GetRotation().y,
				 player->fbxPlayer_Attack->GetRotation().z };

	if (Input::GetInstance()->TriggerKey(DIK_L) && player->GetIsKnock() == false)
	{
		isRote = true;
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
	objSword->SetRotation(rotation);
	
}

void Sword::SwordEnemyCollision(Enemy *enemy, DirectXCommon* dxCommon, Camera* camera)
{
	

	for (int i = 0; i < 3; i++)
	{
		XMVECTOR distanse = { 0, 0, enemyRadius + (enemyRadius * (2.0f * i)) };
		//angleラジアンだけy軸まわりに回転。半径は-100
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
		//angleラジアンだけy軸まわりに回転。半径は-100
		XMMATRIX rotM_Sword = DirectX::XMMatrixIdentity();
		rotM_Sword *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(Angle));
		XMVECTOR v_sowrd = XMVector3TransformNormal(v0_Sword, rotM_Sword);
		XMVECTOR swordVec = { sword3.x, sword3.y, sword3.z };
		XMVECTOR swordPos = { swordVec.m128_f32[0] + v_sowrd.m128_f32[0], swordVec.m128_f32[1] + v_sowrd.m128_f32[1], swordVec.m128_f32[2] + v_sowrd.m128_f32[2] };

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
				isEffect = true;
				
			}
		}
		else
		{
			Decrease = false;
		}
		
	}

	if (isEffect)
	{
		effectTime += 0.1f;
		effects->Play();
		effects->SetPosition({ enemy->GetPosition().x,enemy->GetPosition().y,enemy->GetPosition().z });
		effects->SetScale({ 0.3,0.3,0.3 });
		effects->SetSpeed(5);
		effects->Update(dxCommon->GetCmdList(), camera);
		if (effectTime > effectMaxTime)
		{
			effects->Stop();
			isEffect = false;
			effectTime = 0;
		}
	}
}

void Sword::Draw(DirectXCommon* dxCommon)
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
	effects->Draw(dxCommon->GetCmdList());
}

void Sword::SetPosition(XMFLOAT3 pos)
{
	this->position = pos;
}
