#include "Enemy.h"
#include "Input.h"

using namespace DirectX;

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
	objBoundBullet = Object3d::Create();
	objBoundBullet->InitializeGraphicsPipeline(L"Resource/shaders/OBJVS_Light.hlsl", L"Resource/shaders/OBJPS_Light.hlsl");

	objHomingBullet = Object3d::Create();
	objHomingBullet->InitializeGraphicsPipeline(L"Resource/shaders/OBJVS_Light.hlsl", L"Resource/shaders/OBJPS_Light.hlsl");

	for (int y = 0; y < 10; y++)
	{
		for (int x = 0; x < 13; x++)
		{
			objAirfoilBulletRight[y][x] = Object3d::Create();
			objAirfoilBulletRight[y][x]->InitializeGraphicsPipeline(L"Resource/shaders/OBJVS_Light.hlsl", L"Resource/shaders/OBJPS_Light.hlsl");
			objAirfoilBulletLeft[y][x] = Object3d::Create();
			objAirfoilBulletLeft[y][x]->InitializeGraphicsPipeline(L"Resource/shaders/OBJVS_Light.hlsl", L"Resource/shaders/OBJPS_Light.hlsl");
		}
	}

	objEnemy->SetObject3dModel(enemyModel);
	objEnemy->SetScale({ 0.5,0.5,0.5 });

	objBoundBullet->SetObject3dModel(enemyBulletModel);
	objBoundBullet->SetScale({ 0.5,0.5,0.5 });

	objHomingBullet->SetObject3dModel(enemyBulletModel);
	objHomingBullet->SetScale({ 0.5,0.5,0.5 });

	for (int y = 0; y < 10; y++)
	{
		for (int x = 0; x < 13; x++)
		{
			objAirfoilBulletRight[y][x]->SetObject3dModel(enemyBulletModel);
			objAirfoilBulletRight[y][x]->SetScale({ 0.5,0.5,0.5 });
			objAirfoilBulletLeft[y][x]->SetObject3dModel(enemyBulletModel);
			objAirfoilBulletLeft[y][x]->SetScale({ 0.5,0.5,0.5 });
		}
	}

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
	HomingBullet(player);
	AirfoilBullet(player);
	objEnemy->Update();
	objBoundBullet->Update();
	objHomingBullet->Update();
	for (int y = 0; y < 10; y++)
	{
		for (int x = 0; x < 13; x++)
		{
			objAirfoilBulletRight[y][x]->Update();
			objAirfoilBulletLeft[y][x]->Update();
		}
	}
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
	if (Input::GetInstance()->TriggerKey(DIK_5) && homingBulletFlag == false)
	{

		homingBulletFlag = true;
		homingBulletPos = position;
	}

	if (homingBulletFlag)
	{
		float limitSpeed = 0.3f;     //弾の制限速度
		XMFLOAT3 playerPos = player->GetPosition();
		lengthVec = { playerPos.x - homingBulletPos.x, playerPos.y - homingBulletPos.y, playerPos.z - homingBulletPos.z };  //弾から追いかける対象への方向を計算
		XMVector3Normalize(lengthVec);
		lengthVec = { lengthVec.m128_f32[0] * homingBulletSpeed, lengthVec.m128_f32[1] * homingBulletSpeed, lengthVec.m128_f32[2] * homingBulletSpeed };  //方向の長さを1に正規化、任意の力をAddForceで加える

		float speedXTemp = min(max(lengthVec.m128_f32[0], -limitSpeed), limitSpeed); //X方向の速度を制限
		float speedYTemp = min(max(lengthVec.m128_f32[1], -limitSpeed), limitSpeed);  //Y方向の速度を制限
		float speedZTemp = min(max(lengthVec.m128_f32[2], -limitSpeed), limitSpeed);

		homingBulletPos.x += speedXTemp;
		homingBulletPos.y += speedYTemp;
		homingBulletPos.z += speedZTemp;

		if (homingBulletPos.x == playerPos.x && homingBulletPos.y == playerPos.y && homingBulletPos.z == playerPos.z)
		{
			homingBulletFlag = false;
		}
	}

	

	objHomingBullet->SetPosition(homingBulletPos);
}

void Enemy::BoundBullet(Player* player)
{
	float m1 = 1;
	float vx = 0.1f;
	gravity = 9.8f / 60.0f;
	float vy = 0;
	vy = vy + gravity;//重力の考慮
	boundBulletPos.x = boundBulletPos.x + vx;//速度の更新
	

	/*if (bulletPos.x > 0) {
		vx = vx * -1.0f;
	}*/
	//画面のy座標は逆なので
	//↓が床とのあたり判定
	
	if (gravityFlag)
	{
		boundBulletPos.y = boundBulletPos.y - vy;

		if (boundBulletPos.y < 0)
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

		boundBulletPos.y = boundBulletPos.y + vy;

		if (boundBulletPos.y >= boundHeight) {
			gravityFlag = true;
		}
	}
	

	objBoundBullet->SetPosition(boundBulletPos);
}

void Enemy::AirfoilBullet(Player* player)
{
	for (int y = 0; y < 10; y++)
	{
		for (int x = 0; x < 13; x++)
		{
			airfoilPosRight[y][x] = { 1.0f * x, 2.0f * y, 0 };
			airfoilPosLeft[y][x] = { -1.0f * x, 2.0f * y, 0 };
		}
	}

	for (int y = 0; y < 10; y++)
	{
		for (int x = 0; x < 13; x++)
		{
			objAirfoilBulletRight[y][x]->SetPosition(airfoilPosRight[y][x]);
			objAirfoilBulletLeft[y][x]->SetPosition(airfoilPosLeft[y][x]);
		}
	}
}

void Enemy::Draw()
{
	objEnemy->Draw();
	objBoundBullet->Draw();
	objHomingBullet->Draw();
	for (int y = 0; y < 10; y++)
	{
		for (int x = 0; x < 13; x++)
		{
			objAirfoilBulletRight[y][x]->Draw();
			objAirfoilBulletLeft[y][x]->Draw();
		}
	}
}
