#pragma once

#include <DirectXMath.h>

#include<d3dx12.h>
#include<xaudio2.h>
#include<fstream>
#include"Input.h"
#include"WinApp.h"
#include"DirectXCommon.h"
#include"Object3d.h"
#include"Object3dModel.h"
#include"Sprite.h"
#include"DebugText.h" 
#include"Collision.h"
#include"Audio.h"
#include"ParticleManager.h"
#include"DebugCamera.h"
#include"fbxsdk.h"
#include"FbxLoader.h"
#include"FbxObject3d.h"
#include"PostEffect.h"
#include"Light.h"
#include"Player.h"
#include"Enemy.h"
#include"Sword.h"
#include"Skill.h"

using namespace Microsoft::WRL;
using namespace DirectX;

class GameScene
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
public:
	GameScene();
	~GameScene();

	void Initialize(DirectXCommon* dxc, Audio *sound);
	void Object3dCreate();
	void Resource2dCreate();
	void GameInitialize();
	void Update();
	void ResourcesUpdate();
	void GameUpdate();
	void Draw();
	void Delete();

private:
	DirectXCommon* dxCommon = nullptr;
	ParticleManager* particleMan = nullptr;
	Audio* audio = nullptr;
	DebugText debugText;
	Camera* camera = nullptr;
	Light* light = nullptr;

	Player* player = nullptr;
	Enemy* enemy = nullptr;
	Sword* sword = nullptr;
	Skill* skill = nullptr;


	int debugTextTexNumber = 0;

private://3Dƒ‚ƒfƒ‹
	Object3dModel* modelScene1 = nullptr;
	Object3d* objScene1 = nullptr;
	Object3dModel* modelScene2 = nullptr;
	Object3d* objScene2 = nullptr;
	Object3dModel* modelScene3 = nullptr;
	Object3d* objScene3 = nullptr;
	Object3dModel* modelScene4 = nullptr;
	Object3d* objScene4 = nullptr;
	Object3dModel* Object3dModelGround = nullptr;
	Object3d* objGround = nullptr;
	Object3dModel* Object3dModelSphere = nullptr;
	Object3d* objSphere = nullptr;
	Object3dModel* Object3dModelSphere2 = nullptr;
	Object3d* objSphere2 = nullptr;

	int sceneNo = 0;

private://FBXƒ‚ƒfƒ‹
	FbxModel* model1 = nullptr;
	FbxObject3d* object1 = nullptr;

private://2D
	Sprite* background = nullptr;

public:
	XMFLOAT3 playerPosition = { 1,0,0 };
	float cameraAngle = 0;
};
