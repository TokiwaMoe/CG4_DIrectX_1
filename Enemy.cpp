#include "Enemy.h"
#include "Input.h"

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{
	enemyModel = Object3dModel::LoadFromOBJ("enemy");
	objEnemy = Object3d::Create();
	objEnemy->InitializeGraphicsPipeline(L"Resource/shaders/OBJVS_Light.hlsl", L"Resource/shaders/OBJPS_Light.hlsl");

	objEnemy->SetObject3dModel(enemyModel);
	objEnemy->SetScale({ 0.5,0.5,0.5 });

	easing = new Easing();
	easing->Initialize();

	speed = 0.5f;
}

void Enemy::Update(Player* player)
{
	Target(player);
	Assault(player);
	objEnemy->Update();
}

void Enemy::Move()
{
}

void Enemy::Target(Player* player)
{
	XMFLOAT3 playerPos = player->GetPosition();
	const float direction = 10.0f;
	float AngleX = position.x - playerPos.x;
	float AngleZ = position.z - playerPos.z;
	Angle = (atan2(AngleX, AngleZ)) * 180.0f / 3.14f + direction;

	objEnemy->SetRotation({ 0,Angle,0 });
}

void Enemy::Assault(Player* player)
{
	preAssaultTime++;
	if (preAssaultTime >= maxPreAssaultTime && assaultFlag == false)
	{
		playerOldPos = player->GetPosition();
		oldPos = position;
		assaultFlag = true;
		bfoAssaultTime = 0;
	}

	if (assaultFlag)
	{
		bfoAssaultTime += 0.01f;
		position = easing->ease(oldPos, playerOldPos, bfoAssaultTime);
		preAssaultTime = 0;
		if (bfoAssaultTime >= easing->maxflame)
		{
			assaultFlag = false;
			
		}
	}

	objEnemy->SetPosition(position);
}

void Enemy::Draw()
{
	objEnemy->Draw();
}
