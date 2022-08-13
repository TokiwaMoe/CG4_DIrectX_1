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

	enemyBulletModel = Object3dModel::LoadFromOBJ("sphere");
	objEnemyBullet = Object3d::Create();
	objEnemyBullet->InitializeGraphicsPipeline(L"Resource/shaders/OBJVS_Light.hlsl", L"Resource/shaders/OBJPS_Light.hlsl");

	objEnemy->SetObject3dModel(enemyModel);
	objEnemy->SetScale({ 0.5,0.5,0.5 });

	objEnemyBullet->SetObject3dModel(enemyBulletModel);
	objEnemyBullet->SetScale({ 0.5,0.5,0.5 });

	easing = new Easing();
	easing->Initialize();

	speed = 0.5f;
	boundHeight = 5.0f;
}

void Enemy::Update(Player* player)
{
	Target(player);
	Assault(player);
	BoundBullet(player);
	objEnemy->Update();
	objEnemyBullet->Update();
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

void Enemy::HomingBullet(Player* player)
{
}

void Enemy::BoundBullet(Player* player)
{
	float m1 = 1;
	float vx = 0.1f;
	gravity = 9.8f / 60.0f;
	float vy = 0;
	vy = vy + gravity;//重力の考慮
	bulletPos.x = bulletPos.x + vx;//速度の更新
	

	/*if (bulletPos.x > 0) {
		vx = vx * -1.0f;
	}*/
	//画面のy座標は逆なので
	//↓が床とのあたり判定
	
	if (gravityFlag)
	{
		bulletPos.y = bulletPos.y - vy;

		if (bulletPos.y < 0)
		{
			gravityFlag = false;
			float a1 = m1 * vy;
			float law1 = a1 / m1;
			e1 = vy / law1;
			vy *= e1;
			boundHeight -= 1.0f;
			vx += 0.1f;
		}
	}
	else {

		bulletPos.y = bulletPos.y + vy;

		if (bulletPos.y >= boundHeight) {
			gravityFlag = true;
		}
	}
	

	objEnemyBullet->SetPosition(bulletPos);
}

void Enemy::Draw()
{
	objEnemy->Draw();
	objEnemyBullet->Draw();
}
