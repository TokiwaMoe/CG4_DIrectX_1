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

	void AirfoilBullet(Player* player);

	void Draw();

	const XMFLOAT3& GetPosition() { return position; }


public:
	Object3dModel* enemyModel = nullptr;
	Object3d* objEnemy = nullptr;
	Object3dModel* enemyBulletModel = nullptr;
	Object3d* objBoundBullet = nullptr;
	Object3d* objHomingBullet = nullptr;
	Object3d* objSphere[3];
	int isDraw = 0;
	/*Object3d* objAirfoilBulletRight[10][13];
	Object3d* objAirfoilBulletLeft[10][13];*/

	Easing* easing = nullptr;


	float speed = 0.5f;

	float Angle;

	// ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation = { 0,0,0 };
	// ローカル座標
	XMFLOAT3 position = { 1,0,0 };

	//プレイヤーに向く処理
	float targetTime = 0;
	const float maxTargetTime = 10;
	bool targetFlag = false;

	//突撃
	float preAssaultTime = 0;
	const float maxPreAssaultTime = 100;
	float bfoAssaultTime = 0;
	const float maxBfoAssaultTime = 80;
	bool assaultFlag = false;
	XMFLOAT3 playerOldPos = { 0,0,0 };
	XMFLOAT3 oldPos = { 0,0,0 };

	//バウンド
	float gravity;
	XMFLOAT3 boundBulletPos = { 0,0,0 };
	bool gravityFlag = false;
	float e1;
	float boundHeight;
	XMFLOAT3 endPos = { 0,0,0 };
	float boundTime = 0;

	//ホーミング弾
	XMFLOAT3 homingBulletPos = { 0,0,0 };
	bool homingBulletFlag = false;
	XMVECTOR lengthVec;
	const float homingBulletSpeed = 0.5;
	float homingTime = 0;
	float homingMaxTime = 100;

	//翼型弾
	/*struct RightPoint
	{
		XMFLOAT3 p0[10][13];
		XMFLOAT3 p1[10][13];
		XMFLOAT3 p2[10][13];
		XMFLOAT3 p3[10][13];
	};

	struct LeftPoint
	{
		XMFLOAT3 p0[10][13];
		XMFLOAT3 p1[10][13];
		XMFLOAT3 p2[10][13];
		XMFLOAT3 p3[10][13];
	};

	XMFLOAT3 airfoilPosRight[10][13];
	XMFLOAT3 airfoilPosLeft[10][13];
	RightPoint rightPoint;
	LeftPoint leftPoint;
	XMFLOAT3 a[10][13];
	XMFLOAT3 b[10][13];
	XMFLOAT3 c[10][13];
	XMFLOAT3 d[10][13];
	XMFLOAT3 e[10][13];
	bool airfoilFlag[10];
	XMFLOAT3 playerOldPos_airfoil;
	float airfoilTime = 0;*/

};
