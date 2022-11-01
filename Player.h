#pragma once
#include <DirectXMath.h>
#include"DirectXCommon.h"
#include"Camera.h"
#include"Object3d.h"
#include"Object3dModel.h"
#include"Easing.h"
#include"fbxsdk.h"
#include"FbxLoader.h"
#include"FbxObject3d.h"
#include"DirectXCommon.h"
#include"Effects.h"

class Player {
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
public:
	void Initialize(DirectXCommon *dxCommon, Camera *camera);

	void Init();

	void Update(DirectXCommon* dxCommon, Camera* camera);

	void ResourceUpdate();

	void Move(Camera* camera);

	void defense();

	void defenseKey();

	void defenseMove(XMFLOAT3 FinalPos);

	void Jump();

	void knockBack(DirectXCommon* dxCommon, Camera* camera);

	void Attack();

	void Draw(DirectXCommon* dxCommon);

	void Death();

	const XMFLOAT3& GetPosition() { return position; }

	float SetAngle(float angle) { return cameraAngle = angle; }

	float GetAngle() { return cameraAngle; }

	int GetHP() { return HP; }
	int SetHP(int hp) { return HP = hp; }

	bool SetIsKnock(bool knock) { return isKnock = knock; }

	XMMATRIX GetTransform() { return transform; }

	bool GetIsKnock() { return isKnock; }

	bool GetIsDeath() { return isDeath; }


public:
	std::unique_ptr<FbxModel> player_RunFbxModel = nullptr;
	FbxObject3d* fbxPlayer_Run = nullptr;
	std::unique_ptr<FbxModel> player_DamageFbxModel = nullptr;
	FbxObject3d* fbxPlayer_Damage = nullptr;
	std::unique_ptr<FbxModel> player_WaitFbxModel = nullptr;
	FbxObject3d* fbxPlayer_Wait = nullptr;
	std::unique_ptr<FbxModel> player_DeathFbxModel = nullptr;
	FbxObject3d* fbxPlayer_Death = nullptr;
	std::unique_ptr<FbxModel> player_AttackFbxModel = nullptr;
	FbxObject3d* fbxPlayer_Attack = nullptr;

	Easing* easing = nullptr;
	Effects* effects = nullptr;

	// ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation = { 0,0,0 };
	// プレイヤー座標
	XMFLOAT3 position = { 0,0,0 };
	XMFLOAT3 oldPos = { 0,0,0 };
	XMFLOAT3 endPos = { 0,0,0 };
	XMFLOAT3 storagePos = { 0,0,0 };
	//弾座標
	XMFLOAT3 bulletPos = { 0,0,0 };
	//おとも座標
	XMFLOAT3 friendPos = { 0,0,0 };

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

private:
	//ジャンプ関連の変数
	bool jumpFlag = false;
	const float gravity = 9.8;
	bool gravityFlag = false;

	//回避関連の変数
	bool defenceFlag = false;
	
public:
	int defence_direction;
	float time = 0;
	XMFLOAT3 distance;

	//弾変数
	bool bulletFlag = false;
	const float bulletSpeed = 0.5;
	float bulletTime = 0;
	const float m = 1;	//物量
	const float PI = 3.141592;
	const float angle = 30;
	XMVECTOR lengthVec;
	XMFLOAT3 enemyOldPos = { 0,0,0 };
	Point point;
	XMFLOAT3 a;
	XMFLOAT3 b;
	XMFLOAT3 c;
	XMFLOAT3 d;
	XMFLOAT3 e;

	//おとも
	float otomoAngle = 0;
	float raise = 0.2f;
	bool toggle = false;
	float cameraAngle = 0;

	//ノックバック
	float knockTime = 0;
	XMFLOAT3 knock_EndPos = { 0,0,0 };
	XMFLOAT3 knock_OldPos = { 0,0,0 };
	bool isKnock;
	int HP;
	FbxTime AnimationTime = 0;
	bool AnimetionKnock = false;

	//歩くfbx変数
	bool isWalk = false;
	float rote = 0;
	//ボーン取得変数
	XMMATRIX transform;
	XMVECTOR matBone = { 0,0,0,0 };
	//ゲームオーバーfbx変数
	bool isDeath = false;
	FbxTime deathAnimationTime = 0;
	bool deathAnime = false;
	//アタックfbx変数
	FbxTime attackAnimeTime = 0;
	bool AnimetionAttack = false;
};