#pragma once
#include <DirectXMath.h>
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
#include"WinApp.h"

using namespace Microsoft::WRL;
using namespace DirectX;

class GameScene
{
public:
	GameScene();
	~GameScene();

	void Initialize(WinApp* winApp, DirectXCommon* dxCommon, Input* input, Audio* audio);
	void Update();
	void Draw();
	void Delete();

private:
	WinApp* winApp = nullptr;
	DirectXCommon* dxCommon = nullptr;
	Input* input;
	ParticleManager* particleMan = nullptr;
	Audio* audio = nullptr;
	DebugText debugText;
	DebugCamera* camera = nullptr;

	int debugTextTexNumber = 0;
};
