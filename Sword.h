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

class Sword {
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
public:
	/*Sword();
	~Sword();*/

	void Initialize(Enemy *enemy);

	void Update(Player* player, Enemy *enemy);

	void Move(Player *player);

	void SwordEnemyCollision(Enemy* enemy);

	void Draw();

	void SetPosition(XMFLOAT3 pos);

	bool GetIsHit1() { return isHit_enemy1[12]; }
	bool GetIsHit2() { return isHit_enemy2[12]; }
	bool GetIsHit3() { return isHit_enemy3[12]; }
	XMVECTOR GetCenter() { return swordSphere[0].center; }
	XMFLOAT3 GetCenter_enemy() { return pos[1]; }
	bool GetIsDecrease() { return isDecrease; }
	XMVECTOR GetNormalDirect_SwordA() { return swordOBB.GetDirect(0); }
	XMVECTOR GetNormalDirect_SwordB() { return swordOBB.GetDirect(1); }
	XMVECTOR GetNormalDirect_SwordC() { return swordOBB.GetDirect(2); }

	XMVECTOR GetNormalDirectA() { return enemyOBB.GetDirect(0); }
	XMVECTOR GetNormalDirectB() { return enemyOBB.GetDirect(1); }
	XMVECTOR GetNormalDirectC() { return enemyOBB.GetDirect(2); }
	XMVECTOR GetPositionA() { return swordOBB.GetPos_W(); }
	XMVECTOR GetPositionB() { return enemyOBB.GetPos_W(); }
	float GetAngle() { return Angle; }

public:
	Object3dModel* swordModel = nullptr;
	Object3d* objSword = nullptr;
	Object3dModel* sphereModel = nullptr;
	Object3d* objsphere[13];
	Object3d* objsphere_enemy[3];

	
	// ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation = { 0,0,0 };
	// プレイヤー座標
	XMFLOAT3 position = { 0,0,0 };

	OBB swordOBB;
	OBB enemyOBB;
	

	Sphere swordSphere[13];
	Sphere enemySphere[3];
	const float swordRadius = 0.05f;
	const float enemyRadius = 0.6f;
	bool isHit_enemy1[13];
	bool isHit_enemy2[13];
	bool isHit_enemy3[13];
	bool isDecrease = false;
	float Angle;
	bool isRote = false;
	XMFLOAT3 pos[3];

private:
	bool Attack = false;
};