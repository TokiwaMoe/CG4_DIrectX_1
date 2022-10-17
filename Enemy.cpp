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

	for (int i = 0; i < 3; i++)
	{
		objenemySp[i] = Object3d::Create();
		objenemySp[i]->InitializeGraphicsPipeline(L"Resource/shaders/OBJVS_Light.hlsl", L"Resource/shaders/OBJPS_Light.hlsl");
		objenemySp[i]->SetObject3dModel(enemyBulletModel);
		objenemySp[i]->SetScale({ enemyRad,enemyRad,enemyRad });
	}

	for (int i = 0; i < 2; i++)
	{
		objplayerSp[i] = Object3d::Create();
		objplayerSp[i]->InitializeGraphicsPipeline(L"Resource/shaders/OBJVS_Light.hlsl", L"Resource/shaders/OBJPS_Light.hlsl");
		objplayerSp[i]->SetObject3dModel(enemyBulletModel);
		objplayerSp[i]->SetScale({ playerRad,playerRad,playerRad });
	}

	/*for (int y = 0; y < 10; y++)
	{
		for (int x = 0; x < 13; x++)
		{
			objAirfoilBulletRight[y][x] = Object3d::Create();
			objAirfoilBulletRight[y][x]->InitializeGraphicsPipeline(L"Resource/shaders/OBJVS_Light.hlsl", L"Resource/shaders/OBJPS_Light.hlsl");
			objAirfoilBulletLeft[y][x] = Object3d::Create();
			objAirfoilBulletLeft[y][x]->InitializeGraphicsPipeline(L"Resource/shaders/OBJVS_Light.hlsl", L"Resource/shaders/OBJPS_Light.hlsl");
		}
	}*/

	objEnemy->SetObject3dModel(enemyModel);
	objEnemy->SetScale({ 0.5,0.5,0.5 });

	objBoundBullet->SetObject3dModel(enemyBulletModel);
	objBoundBullet->SetScale({ 0.5,0.5,0.5 });

	objHomingBullet->SetObject3dModel(enemyBulletModel);
	objHomingBullet->SetScale({ 0.5,0.5,0.5 });

	/*for (int y = 0; y < 10; y++)
	{
		for (int x = 0; x < 13; x++)
		{
			objAirfoilBulletRight[y][x]->SetObject3dModel(enemyBulletModel);
			objAirfoilBulletRight[y][x]->SetScale({ 0.5,0.5,0.5 });
			objAirfoilBulletLeft[y][x]->SetObject3dModel(enemyBulletModel);
			objAirfoilBulletLeft[y][x]->SetScale({ 0.5,0.5,0.5 });
		}
	}*/

	easing = new Easing();
	easing->Initialize();

	speed = 0.5f;
	boundHeight = 5.0f;

	/*for (int i = 0; i < 10; i++)
	{
		airfoilFlag[i] = false;
	}*/
}

void Enemy::Update(Player* player)
{
	Target(player);
	Assault(player);
	BoundBullet(player);
	HomingBullet(player);
	PlayerenemyCollision(player);
	//AirfoilBullet(player);
	objEnemy->Update();
	objBoundBullet->Update();
	objHomingBullet->Update();

	for (int i = 0; i < 3; i++)
	{
		objenemySp[i]->Update();
	}

	for (int i = 0; i < 2; i++)
	{
		objplayerSp[i]->Update();
	}
	
	/*for (int y = 0; y < 10; y++)
	{
		for (int x = 0; x < 13; x++)
		{
			objAirfoilBulletRight[y][x]->Update();
			objAirfoilBulletLeft[y][x]->Update();
		}
	}*/
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
		oldPos = { position.x, 0, position.z };
		assaultFlag = true;
		bfoAssaultTime = 0;
	}

	if (assaultFlag)
	{
		bfoAssaultTime += 0.01f;
		position = easing->ease(oldPos, playerOldPos, bfoAssaultTime);
		preAssaultTime = 0;

		
		//isHit = Collision::CheckSphere2(playerSphere, enemySphere);
		
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
		float limitSpeed = 0.3f;     //’e‚Ì§ŒÀ‘¬“x
		XMFLOAT3 playerPos = player->GetPosition();
		lengthVec = { playerPos.x - homingBulletPos.x, playerPos.y - homingBulletPos.y, playerPos.z - homingBulletPos.z };  //’e‚©‚ç’Ç‚¢‚©‚¯‚é‘ÎÛ‚Ö‚Ì•ûŒü‚ðŒvŽZ
		XMVector3Normalize(lengthVec);
		lengthVec = { lengthVec.m128_f32[0] * homingBulletSpeed, lengthVec.m128_f32[1] * homingBulletSpeed, lengthVec.m128_f32[2] * homingBulletSpeed };  //•ûŒü‚Ì’·‚³‚ð1‚É³‹K‰»A”CˆÓ‚Ì—Í‚ðAddForce‚Å‰Á‚¦‚é

		float speedXTemp = min(max(lengthVec.m128_f32[0], -limitSpeed), limitSpeed); //X•ûŒü‚Ì‘¬“x‚ð§ŒÀ
		float speedYTemp = min(max(lengthVec.m128_f32[1], -limitSpeed), limitSpeed);  //Y•ûŒü‚Ì‘¬“x‚ð§ŒÀ
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
	
	endPos = { 60,0,0 };
	boundBulletPos = { 0,30,0 };
	boundBulletPos = easing->easeOut_Bounce(boundBulletPos, endPos, boundTime);
	boundTime += 0.01f;

	if (boundTime >= easing->maxflame)
	{
		boundBulletPos = endPos;
		boundTime = 0;
	}

	objBoundBullet->SetPosition(boundBulletPos);
}

void Enemy::AirfoilBullet(Player* player)
{
	/*for (int i = 0; i < 10; i++)
	{
		if (Input::GetInstance()->TriggerKey(DIK_6) && airfoilFlag[i] == false)
		{
			for (int y = 0; y < 10; y++)
			{
				for (int x = 0; x < 13; x++)
				{
					airfoilPosRight[y][x] = { position.x + 2.0f * x, position.x + 2.0f * y, position.z};
					airfoilPosLeft[y][x] = { position.x - 2.0f * x, position.y + 2.0f * y, position.z };
					playerOldPos_airfoil = player->GetPosition();
					airfoilTime = 0;
					airfoilFlag[i] = true;
					rightPoint.p0[y][x] = airfoilPosRight[y][x];
					rightPoint.p1[y][x] = { rightPoint.p0[y][x].x + 0.5f, rightPoint.p0[y][x].y + 2.0f, rightPoint.p0[y][x].z - 0.5f };
					rightPoint.p2[y][x] = { rightPoint.p1[y][x].x + 0.5f, rightPoint.p1[y][x].y, rightPoint.p1[y][x].z - 0.5f };
					rightPoint.p3[y][x] = playerOldPos_airfoil;
				}
			}
		}

		if (airfoilFlag[i])
		{
			for (int y = 0; y < 10; y++)
			{
				for (int x = 0; x < 13; x++)
				{
					airfoilTime += 0.05f;
					a[y][x] = easing->lerp(rightPoint.p0[y][x], rightPoint.p1[y][x], airfoilTime);
					b[y][x] = easing->lerp(rightPoint.p1[y][x], rightPoint.p2[y][x], airfoilTime);
					c[y][x] = easing->lerp(rightPoint.p2[y][x], rightPoint.p3[y][x], airfoilTime);
					d[y][x] = easing->lerp(a[y][x], b[y][x], airfoilTime);
					e[y][x] = easing->lerp(b[y][x], c[y][x], airfoilTime);
					airfoilPosRight[y][x] = easing->lerp(d[y][x], e[y][x], airfoilTime);
					if (airfoilTime >= 1.0f)
					{
						airfoilFlag[i] = false;

					}
				}
			}
		}
	}

	

	for (int y = 0; y < 10; y++)
	{
		for (int x = 0; x < 13; x++)
		{
			objAirfoilBulletRight[y][x]->SetPosition(airfoilPosRight[y][x]);
			objAirfoilBulletLeft[y][x]->SetPosition(airfoilPosLeft[y][x]);
		}
	}*/
}

void Enemy::Draw()
{
	
	
	objBoundBullet->Draw();
	objHomingBullet->Draw();

	objEnemy->Draw();

	for (int i = 0; i < 2; i++)
	{
		objplayerSp[i]->Draw();
	}

	for (int i = 0; i < 3; i++)
	{
		objenemySp[i]->Draw();
	}
	
	
	
	/*for (int y = 0; y < 10; y++)
	{
		for (int x = 0; x < 13; x++)
		{
			objAirfoilBulletRight[y][x]->Draw();
			objAirfoilBulletLeft[y][x]->Draw();
		}
	}*/
}

void Enemy::PlayerenemyCollision(Player* player)
{
	for (int i = 0; i < 3; i++)
	{
		XMVECTOR distanse = { 0, 0, enemyRad + (enemyRad * (2.0f * i)) };
		//angleƒ‰ƒWƒAƒ“‚¾‚¯yŽ²‚Ü‚í‚è‚É‰ñ“]B”¼Œa‚Í-100
		XMMATRIX rotM_Enemy = DirectX::XMMatrixIdentity();
		rotM_Enemy *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(Angle));
		XMVECTOR v_enemy = XMVector3TransformNormal(distanse, rotM_Enemy);
		XMVECTOR enemyVec = { position.x, position.y, position.z };
		XMVECTOR enemyPos = { enemyVec.m128_f32[0] + v_enemy.m128_f32[0], enemyVec.m128_f32[1] + v_enemy.m128_f32[1], enemyVec.m128_f32[2] + v_enemy.m128_f32[2] };

		enemySphere[i].center = enemyPos;
		objenemySp[i]->SetPosition({ enemySphere[i].center.m128_f32[0], enemySphere[i].center.m128_f32[1], enemySphere[i].center.m128_f32[2] });
	}


	for (int i = 0; i < 2; i++)
	{
		XMVECTOR v0_Player = { 0, playerRad * 2 * i, 0 };
		//angleƒ‰ƒWƒAƒ“‚¾‚¯yŽ²‚Ü‚í‚è‚É‰ñ“]B”¼Œa‚Í-100
		XMMATRIX rotM_Player = DirectX::XMMatrixIdentity();
		rotM_Player *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(player->GetAngle()));
		XMVECTOR v = XMVector3TransformNormal(v0_Player, rotM_Player);
		XMVECTOR playerVec = { player->GetPosition().x, player->GetPosition().y, player->GetPosition().z};
		XMVECTOR playerPos = { playerVec.m128_f32[0] + v.m128_f32[0], playerVec.m128_f32[1] + v.m128_f32[1], playerVec.m128_f32[2] + v.m128_f32[2] };

		playerSphere[i].center = playerPos;
		playerSphere[i].radius = playerRad;
		objplayerSp[i]->SetPosition({ playerSphere[i].center.m128_f32[0], playerSphere[i].center.m128_f32[1], playerSphere[i].center.m128_f32[2] });
	}



	/*for (int i = 0; i < 13; i++)
	{
		isHit_enemy1[i] = Collision::CheckSphere2(playerSphere[i], enemySphere[0]);
		isHit_enemy2[i] = Collision::CheckSphere2(playerSphere[i], enemySphere[1]);
		isHit_enemy3[i] = Collision::CheckSphere2(playerSphere[i], enemySphere[2]);
	}*/
}
