#include "Sword.h"
#include "Input.h"

void Sword::Initialize()
{
	swordModel = Object3dModel::LoadFromOBJ("sword");
	objSword = Object3d::Create();
	objSword->InitializeGraphicsPipeline(L"Resource/shaders/OBJVS_Light.hlsl", L"Resource/shaders/OBJPS_Light.hlsl");
	

	objSword->SetObject3dModel(swordModel);
	objSword->SetScale({ 0.02,0.02,0.02 });
	objSword->SetRotation({ 90,0,0 });
	
}

void Sword::Update(Player* player)
{
	objSword->SetRotation({ 90,0,0 });
	Move(player);
	objSword->Update();
}

void Sword::Move(Player* player)
{
	position = player->GetPosition();

	if (Input::GetInstance()->PushKey(DIK_4))
	{
		position.x -= 1;
		position.y -= 1;
		objSword->SetRotation({ 90, 0, 30 });
	}
	
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
