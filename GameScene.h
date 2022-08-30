#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include <string>
#include <cassert>
#include<sstream>
#include<iomanip>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#define DIRECTINPUT_VERSION   0x0800
#include <dinput.h>
#include <math.h>
#include <DirectXTex.h>
#include<wrl.h>
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
public:
	GameScene();
	~GameScene();

	void Initialize(WinApp* winApp);
	void Object3dCreate();
	void Resource2dCreate();
	void GameInitialize();
	void Update();
	void ResourcesUpdate();
	void GameUpdate();
	void Draw();
	void ResourceDraw();
	void GameDraw();
	void Delete();

private:
	DirectXCommon* dxCommon = nullptr;
	Input* input;
	ParticleManager* particleMan = nullptr;
	Audio* audio = nullptr;
	DebugText debugText;
	DebugCamera* camera = nullptr;
	ParticleManager* particleMan = nullptr;
	DebugText debugText;
	Light* light = nullptr;
	Player* player = nullptr;
	Enemy* enemy = nullptr;
	Sword* sword = nullptr;
	Skill* skill = nullptr;


	int debugTextTexNumber = 0;

private://3Dƒ‚ƒfƒ‹
	Object3dModel* Object3dModelSkydome;
	Object3d* objSkydome;
	Object3dModel* Object3dModelGround;
	Object3d* objGround;
	Object3dModel* Object3dModelSphere;
	Object3d* objSphere;
	Object3dModel* Object3dModelSphere2;
	Object3d* objSphere2;

private://FBXƒ‚ƒfƒ‹
	FbxModel* model1 = nullptr;
	FbxObject3d* object1 = nullptr;

private://2D
	Sprite* background = nullptr;

public:
	XMFLOAT3 playerPosition = { 1,0,0 };
};
