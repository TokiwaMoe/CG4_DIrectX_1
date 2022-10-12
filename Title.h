#pragma once
#include"Sprite.h"
#include"DebugText.h"
#include"Object3d.h"
#include"Object3dModel.h"
#include"DirectXCommon.h"

class Title
{
public:
	void Initialize(DirectXCommon* dxc);
	void Update();
	void Draw();

private:
	DebugText debugText;
	DirectXCommon* dxCommon = nullptr;
	int debugTextTexNumber = 0;
};
