#pragma once
#include <DirectXMath.h>
#include"DirectXCommon.h"
#include"Camera.h"
#include"Object3d.h"
#include"Object3dModel.h"
#include"Easing.h"
#include"Player.h"
#include"Enemy.h"
#include"Collision.h"
#include"CollisionPrimitive.h"

class Skill {
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
public:
	void Initialize();

	void Update(Player* player, Enemy *enemy);

	void Move(Player *player);

	void HomingBullet(Player* player, Enemy *enemy);

	void Draw();

	void bulletEnemyCollision(Enemy* enemy);

	const XMFLOAT3& GetPosition() { return position; }


public:
	Easing* easing = nullptr;
	Player* player = nullptr;

	Object3dModel* friendModel = nullptr;
	Object3d* objFriend = nullptr;
	Object3dModel* bulletModel = nullptr;
	Object3d* objBullet[5];
	Object3d* objPlayerBullet = nullptr;

	// ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation = { 0,0,0 };
	//座標
	XMFLOAT3 position = { 0,0,0 };
	//弾座標
	XMFLOAT3 bulletPos[5];

	enum direction
	{
		Previous,
		Back,
		Right,
		Left
	};

	struct Point
	{
		XMFLOAT3 p0 = { 0,0,0 };
		XMFLOAT3 p1 = { 0,0,0 };
		XMFLOAT3 p2 = { 0,0,0 };
		XMFLOAT3 p3 = { 0,0,0 };
	};

public:
	//弾変数
	bool bulletFlag[5];
	const float bulletSpeed = 0.5;
	float bulletTime[5];
	const float m = 1;	//物量
	const float PI = 3.141592;
	const float angle = 30;
	XMVECTOR lengthVec;
	XMFLOAT3 enemyOldPos = { 0,0,0 };
	Point point[5];
	XMFLOAT3 a[5];
	XMFLOAT3 b[5];
	XMFLOAT3 c[5];
	XMFLOAT3 d[5];
	XMFLOAT3 e[5];

	//おとも
	float otomoAngle = 0;
	float raise = 0.2f;
	bool toggle = false;

	Sphere bulletSp[5];
	Sphere enemySp;
	float bulletRad = 0.1f;
	float enemyRad = 0.6f;
	bool isHit = false;

};