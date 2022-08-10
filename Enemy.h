#pragma once
#include"DirectXCommon.h"
#include"Camera.h"
#include"Object3d.h"
#include"Object3dModel.h"
#include"Player.h"

class Enemy
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	Enemy();
	~Enemy();
	void Initialize();

	void Update(Player* player);

	void Move();

	void Target(Player* player);

	void Draw();


public:
	Object3dModel* enemyModel = nullptr;
	Object3d* objEnemy = nullptr;

	Player* player = nullptr;
	float speed = 0.5f;

	float Angle;

	// ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation = { 0,0,0 };
	// ローカル座標
	XMFLOAT3 position = { 1,0,0 };
};
