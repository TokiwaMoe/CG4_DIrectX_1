#pragma once
#include"DirectXCommon.h"
#include"Camera.h"
#include"Object3d.h"
#include"Object3dModel.h"
#include"Player.h"
#include"easing.h"

class Enemy
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
public:
	Enemy();
	~Enemy();
	void Initialize();

	void Update(Player* player);

	void Move();

	void Target(Player* player);

	void Assault(Player* player);

	void HomingBullet(Player* player);

	void BoundBullet(Player* player);

	void Draw();

	const XMFLOAT3& GetPosition() { return position; }


public:
	Object3dModel* enemyModel = nullptr;
	Object3d* objEnemy = nullptr;
	Object3dModel* enemyBulletModel = nullptr;
	Object3d* objBoundBullet = nullptr;
	Object3d* objHomingBullet = nullptr;

	Easing* easing = nullptr;


	float speed = 0.5f;

	float Angle;

	// ���[�J���X�P�[��
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z�����̃��[�J����]�p
	XMFLOAT3 rotation = { 0,0,0 };
	// ���[�J�����W
	XMFLOAT3 position = { 1,0,0 };

	//�v���C���[�Ɍ�������
	float targetTime = 0;
	const float maxTargetTime = 10;
	bool targetFlag = false;

	//�ˌ�
	float preAssaultTime = 0;
	const float maxPreAssaultTime = 100;
	float bfoAssaultTime = 0;
	const float maxBfoAssaultTime = 80;
	bool assaultFlag = false;
	XMFLOAT3 playerOldPos = { 0,0,0 };
	XMFLOAT3 oldPos = { 0,0,0 };

	//�o�E���h
	float gravity;
	XMFLOAT3 boundBulletPos = { 0,0,0 };
	bool gravityFlag = false;
	float e1;
	float boundHeight;

	//�z�[�~���O�e
	XMFLOAT3 homingBulletPos = { 0,0,0 };
	bool homingBulletFlag = false;
	XMVECTOR lengthVec;
	const float homingBulletSpeed = 0.5;
	float homingTime = 0;
	float homingMaxTime = 100;
};
