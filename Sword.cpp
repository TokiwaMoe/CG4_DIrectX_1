#include "Sword.h"
#include "Input.h"
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>

<<<<<<< HEAD
void Sword::Initialize(Enemy *enemy, DirectXCommon* dxCommon, Camera* camera)
=======
using namespace DirectX;

void Sword::Initialize(Enemy *enemy)
>>>>>>> OBB
{
	swordModel = Object3dModel::LoadFromOBJ("sword");
	objSword = Object3d::Create();
	objSword->InitializeGraphicsPipeline(L"Resource/shaders/OBJVS_Light.hlsl", L"Resource/shaders/OBJPS_Light.hlsl");
	objSword->SetObject3dModel(swordModel);
	objSword->SetScale({ 0.01,0.01,0.01 });
	objSword->SetRotation({ 0,0,0 });

	sphereModel = Object3dModel::LoadFromOBJ("sphere");
	for (int i = 0; i < sword_Max; i++)
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
	effects->Load(L"effectsTest/10/fireflower.efk");
}

void Sword::Update(Player* player, Enemy *enemy, DirectXCommon* dxCommon, Camera* camera)
{
	Move(player);
	objSword->Update();
<<<<<<< HEAD
	for (int i = 0; i < sword_Max; i++)
=======

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
>>>>>>> 5399056b55443f793de1dd2d3c84aaf0a9e80bd7
	{
		objsphere[i]->Update();
	}
	
	for (int i = 0; i < 3; i++)
	{
		
		objsphere_enemy[i]->Update();
	}
	
	SwordEnemyCollision(enemy, dxCommon, camera, player);
}

void Sword::Move(Player* player)
{
<<<<<<< HEAD
	
	// ボーンの計算済みワールド行列（ボーンのローカル行列×プレイヤーのワールド行列）
	boneWorldMatrix = player->GetTransform() * player->GetMatWorld();

	position = {
		boneWorldMatrix.r[3].m128_f32[0],
		boneWorldMatrix.r[3].m128_f32[1],
		boneWorldMatrix.r[3].m128_f32[2]
	};

	rotation = { player->fbxPlayer_Attack->GetRotation().x,
				 player->fbxPlayer_Attack->GetRotation().y,
				 player->fbxPlayer_Attack->GetRotation().z };
=======
<<<<<<< HEAD
	/*if (player->isWalk)
	{
		position = {
		player->GetPosition().x + 0.1f + player->GetTransform().r[3].m128_f32[0] / 100.0f,
		player->GetPosition().y + 0.7f - player->GetTransform().r[3].m128_f32[1] / 100.0f,
		player->GetPosition().z + 0.3f - player->GetTransform().r[3].m128_f32[2] / 100.0f
		};
	}
	else if (player->AnimetionKnock)
	{
		position = {
		player->GetPosition().x + 0.1f + player->GetTransform().r[3].m128_f32[0] / 100.0f,
		player->GetPosition().y + 0.5f - player->GetTransform().r[3].m128_f32[1] / 100.0f,
		player->GetPosition().z + 0.3f - player->GetTransform().r[3].m128_f32[2] / 100.0f
		};
	}
	else if (player->AnimetionAttack)
	{
		position = {
		player->GetPosition().x + 0.1f + player->GetTransform().r[3].m128_f32[0] / 100.0f,
		player->GetPosition().y + 0.7f - player->GetTransform().r[3].m128_f32[1] / 100.0f,
		player->GetPosition().z + 0.3f - player->GetTransform().r[3].m128_f32[2] / 100.0f
		};
	}*/
	//else
	{
		position = {
		player->GetPosition().x + 0.1f + player->GetTransform().r[3].m128_f32[0] / 100.0f,
		player->GetPosition().y + 0.5f - player->GetTransform().r[3].m128_f32[1] / 100.0f,
		player->GetPosition().z - 0.35f - player->GetTransform().r[3].m128_f32[2] / 100.0f
		};
	}
	

	XMVECTOR distanse = { 0.2f, 0.2f, -0.2f };
	//angleラジアンだけy軸まわりに回転。半径は-100
	XMMATRIX rotM = DirectX::XMMatrixIdentity();
	rotM *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(player->fbxPlayer_Run->GetRotation().y + player->GetAngle()));
	XMVECTOR v = XMVector3TransformNormal(distanse, rotM);
	XMVECTOR f3 = { position.x, position.y, position.z };
	XMVECTOR swordVec = { f3.m128_f32[0] + v.m128_f32[0], f3.m128_f32[1] + v.m128_f32[1], f3.m128_f32[2] + v.m128_f32[2] };
	sword3 = { swordVec.m128_f32[0], swordVec.m128_f32[1], swordVec.m128_f32[2] };
=======
	position = { player->GetPosition().x, player->GetPosition().y, player->GetPosition().z };
	XMVECTOR v = { 0,0,0,0 };
	XMVECTOR sword_vector = { 0,0.1,0,0 };//x
	XMMATRIX matRot_Sword = XMMatrixIdentity();
	matRot_Sword *= XMMatrixRotationX(XMConvertToRadians(Angle));//y軸を中心に回転するマトリックスを作成
	v = XMVector3TransformNormal(sword_vector, matRot_Sword);
	XMVECTOR v3 = { 0,0,0,0 };
>>>>>>> OBB

	rotation = {  player->GetTransform().r[2].m128_f32[0] * 100,
				 player->GetTransform().r[2].m128_f32[1] * 100,
				 player->GetTransform().r[2].m128_f32[2] * 100 };

	if (Input::GetInstance()->TriggerKey(DIK_L) && player->GetIsKnock() == false)
	{
		isRote = true;
	}
>>>>>>> 5399056b55443f793de1dd2d3c84aaf0a9e80bd7
	
	if (player->defence_direction == player->Previous)
	{
<<<<<<< HEAD
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
=======
		Angle += 5.0f;
		v3 = v + swordOBB.m_Pos;
		if (Angle >= 90)
		{
			Angle = 0;
			isRote = false;
		}
>>>>>>> OBB
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
	
	objSword->SetPosition(position);
	objSword->SetRotation(rotation);
	
}

void Sword::SwordEnemyCollision(Enemy *enemy, DirectXCommon* dxCommon, Camera* camera, Player *player)
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
	
	
	for (int i = 0; i < sword_Max; i++)
	{
		XMVECTOR v0_Sword = { 0, 0.1f, swordRadius * 2 * i };
		//angleラジアンだけy軸まわりに回転。半径は-100
		XMMATRIX rotM_Sword = DirectX::XMMatrixIdentity();
		rotM_Sword *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(rotation.x));
		rotM_Sword *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(rotation.y));
		rotM_Sword *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(rotation.z));
		XMVECTOR v_sowrd = XMVector3TransformNormal(v0_Sword, rotM_Sword);
		XMVECTOR swordVec = { position.x, position.y, position.z };
		XMVECTOR swordPos = { swordVec.m128_f32[0] + v_sowrd.m128_f32[0], swordVec.m128_f32[1] + v_sowrd.m128_f32[1], swordVec.m128_f32[2] + v_sowrd.m128_f32[2] };

		swordSphere[i].center = swordPos;
		swordSphere[i].radius = swordRadius;
		objsphere[i]->SetPosition({ swordSphere[i].center.m128_f32[0], swordSphere[i].center.m128_f32[1], swordSphere[i].center.m128_f32[2] });
	}

	

	for (int i = 0; i < sword_Max; i++)
	{
		isHit_enemy1[i] = Collision::CheckSphere2(swordSphere[i], enemySphere[0]);
		isHit_enemy2[i] = Collision::CheckSphere2(swordSphere[i], enemySphere[1]);
		isHit_enemy3[i] = Collision::CheckSphere2(swordSphere[i], enemySphere[2]);
	
		if (player->GetIsAttack())
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
	for (int i = 0; i < 13; i++)
	{
		objsphere[i]->Draw();
	}

	for (int i = 0; i < 3; i++)
	{
		objsphere_enemy[i]->Draw();
	}
	effects->Draw(dxCommon->GetCmdList());
}

void Sword::SetPosition(XMFLOAT3 pos)
{
	this->position = pos;
}
