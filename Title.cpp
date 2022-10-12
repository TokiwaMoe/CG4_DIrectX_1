#include"Title.h"

void Title::Initialize(DirectXCommon* dxc)
{
	Sprite::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);

	debugText.Initialize(debugTextTexNumber);
	Sprite::LoadTexture(debugTextTexNumber, L"Resource/debugfon.png");
}

void Title::Update()
{

}

void Title::Draw()
{

}