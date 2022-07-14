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
	enemyModel = Object3dModel::LoadFromOBJ("sphere");
	objEnemy = Object3d::Create();
	objEnemy->InitializeGraphicsPipeline(L"Resource/shaders/OBJVS_Light.hlsl", L"Resource/shaders/OBJPS_Light.hlsl");

	objEnemy->SetObject3dModel(enemyModel);
	objEnemy->SetScale({ 0.5,0.5,0.5 });

	speed = 0.5f;
}

void Enemy::Update()
{
	objEnemy->Update();
}

void Enemy::Move()
{
}

void Enemy::Draw()
{
	objEnemy->Draw();
}
