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
#include"Effects.h"

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

	void Initialize(Enemy *enemy, DirectXCommon* dxCommon, Camera* camera);

	void Update(Player* player, Enemy *enemy, DirectXCommon* dxCommon, Camera* camera);

	void Move(Player *player);

	void SwordEnemyCollision(Enemy* enemy, DirectXCommon* dxCommon, Camera* camera, Player *player);

	void Draw(DirectXCommon* dxCommon);

	void SetPosition(XMFLOAT3 pos);

	bool GetIsHit1() { return isHit_enemy1[sword_Max]; }
	bool GetIsHit2() { return isHit_enemy2[sword_Max]; }
	bool GetIsHit3() { return isHit_enemy3[sword_Max]; }
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
	XMFLOAT3 GetRotation() { return rotation; }

private:
	static const int sword_Max = 10;

public:
	Object3dModel* swordModel = nullptr;
	Object3d* objSword = nullptr;
	Object3dModel* sphereModel = nullptr;
	Object3d* objsphere[sword_Max];
	Object3d* objsphere_enemy[3];

	Effects* effects = nullptr;

	
	// ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation = { 0,0,0 };
	// プレイヤー座標
	XMFLOAT3 position = { 0,0,0 };

	OBB swordOBB;
	OBB enemyOBB;
	bool Decrease = false;

	Sphere swordSphere[sword_Max];
	Sphere enemySphere[3];
	const float swordRadius = 0.05f;
	const float enemyRadius = 0.6f;
	bool isHit_enemy1[sword_Max];
	bool isHit_enemy2[sword_Max];
	bool isHit_enemy3[sword_Max];
	bool isDecrease = false;
	float Angle;
	bool isRote = false;
	XMFLOAT3 pos[3];
	XMFLOAT3 sword3 = { 0,0,0 };

	float effectTime = 0;
	const float effectMaxTime = 1;
	bool isEffect = false;

	XMMATRIX boneWorldMatrix;

private:
	bool Attack = false;
};