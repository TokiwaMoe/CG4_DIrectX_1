#include "Sword.h"

void Sword::Initialize()
{
	swordModel = Object3dModel::LoadFromOBJ("sword");
	objSword = Object3d::Create();
	objSword->InitializeGraphicsPipeline(L"Resource/shaders/OBJVS_Light.hlsl", L"Resource/shaders/OBJPS_Light.hlsl");
	

	objSword->SetObject3dModel(swordModel);
	objSword->SetScale({ 0.02,0.02,0.02 });
	
}

void Sword::Update(Player* player)
{
	Move(player);
	objSword->Update();
}

void Sword::Move(Player* player)
{
	position = player->GetPosition();
	objSword->SetPosition(position);
}

void Sword::Draw()
{
	objSword->Draw();
}

void Sword::SetPosition(XMFLOAT3 pos)
{
	this->position = pos;
}
