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

	// ���[�J���X�P�[��
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z�����̃��[�J����]�p
	XMFLOAT3 rotation = { 0,0,0 };
	// �v���C���[���W
	XMFLOAT3 position = { 0,0,0 };
	XMFLOAT3 oldPos = { 0,0,0 };
	XMFLOAT3 endPos = { 0,0,0 };
	XMFLOAT3 storagePos = { 0,0,0 };
	//�e���W
	XMFLOAT3 bulletPos = { 0,0,0 };
	//���Ƃ����W
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
	//�W�����v�֘A�̕ϐ�
	bool jumpFlag = false;
	const float gravity = 9.8;
	bool gravityFlag = false;

	//����֘A�̕ϐ�
	bool defenceFlag = false;
	
public:
	int defence_direction;
	float time = 0;
	XMFLOAT3 distance;

	//�e�ϐ�
	bool bulletFlag = false;
	const float bulletSpeed = 0.5;
	float bulletTime = 0;
	const float m = 1;	//����
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

	//���Ƃ�
	float otomoAngle = 0;
	float raise = 0.2f;
	bool toggle = false;
	float cameraAngle = 0;

	//�m�b�N�o�b�N
	float knockTime = 0;
	XMFLOAT3 knock_EndPos = { 0,0,0 };
	XMFLOAT3 knock_OldPos = { 0,0,0 };
	bool isKnock;
	int HP;
	FbxTime AnimationTime = 0;
	bool AnimetionKnock = false;

	//����fbx�ϐ�
	bool isWalk = false;
	float rote = 0;
	//�{�[���擾�ϐ�
	XMMATRIX transform;
	XMVECTOR matBone = { 0,0,0,0 };
	//�Q�[���I�[�o�[fbx�ϐ�
	bool isDeath = false;
	FbxTime deathAnimationTime = 0;
	bool deathAnime = false;
	//�A�^�b�Nfbx�ϐ�
	FbxTime attackAnimeTime = 0;
	bool AnimetionAttack = false;
};