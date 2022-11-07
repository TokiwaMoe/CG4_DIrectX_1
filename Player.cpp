#include "Player.h"
#include <time.h>
#include <cassert>
#include "SphereCollider.h"
#include "Input.h"

using namespace DirectX;

void Player::Initialize(DirectXCommon* dxCommon, Camera* camera)
{

	playerModel = Object3dModel::LoadFromOBJ("player");
	objPlayer = Object3d::Create();
	objPlayer->InitializeGraphicsPipeline(L"Resource/shaders/OBJVS_Light.hlsl", L"Resource/shaders/OBJPS_Light.hlsl");

	objPlayer->SetObject3dModel(playerModel);
	objPlayer->SetScale({ 0.05,0.05,0.05 });

	player_RunFbxModel = FbxLoader::GetInstance()->LoadMadelFromFile("player_Run");
	fbxPlayer_Run = new FbxObject3d;
	fbxPlayer_Run->Initialize();
	fbxPlayer_Run->SetModel(player_RunFbxModel.get());
	fbxPlayer_Run->PlayAnimation();
	fbxPlayer_Run->SetScale({ 0.01,0.01,0.01 });
	fbxPlayer_Run->SetPosition({ 0,0, 5.5 });

	player_DamageFbxModel = FbxLoader::GetInstance()->LoadMadelFromFile("player_Damage");
	fbxPlayer_Damage = new FbxObject3d;
	fbxPlayer_Damage->Initialize();
	fbxPlayer_Damage->SetModel(player_DamageFbxModel.get());
	fbxPlayer_Damage->PlayAnimation();
	fbxPlayer_Damage->SetScale({ 0.01,0.01,0.01 });
	fbxPlayer_Damage->SetPosition({ 0,0, 5.5 });

	player_WaitFbxModel = FbxLoader::GetInstance()->LoadMadelFromFile("player_Wait");
	fbxPlayer_Wait = new FbxObject3d;
	fbxPlayer_Wait->Initialize();
	fbxPlayer_Wait->SetModel(player_WaitFbxModel.get());
	fbxPlayer_Wait->PlayAnimation();
	fbxPlayer_Wait->SetScale({ 0.01,0.01,0.01 });
	fbxPlayer_Wait->SetPosition({ 0,0, 5.5 });

	player_DeathFbxModel = FbxLoader::GetInstance()->LoadMadelFromFile("player_Death");
	fbxPlayer_Death = new FbxObject3d;
	fbxPlayer_Death->Initialize();
	fbxPlayer_Death->SetModel(player_DeathFbxModel.get());
	fbxPlayer_Death->PlayAnimation();
	fbxPlayer_Death->SetScale({ 0.01,0.01,0.01 });
	fbxPlayer_Death->SetPosition({ 0,0, 5.5 });

	player_AttackFbxModel = FbxLoader::GetInstance()->LoadMadelFromFile("player_Attack");
	fbxPlayer_Attack = new FbxObject3d;
	fbxPlayer_Attack->Initialize();
	fbxPlayer_Attack->SetModel(player_AttackFbxModel.get());
	fbxPlayer_Attack->PlayAnimation();
	fbxPlayer_Attack->SetScale({ 0.01,0.01,0.01 });
	fbxPlayer_Attack->SetPosition({ 0,0, 5.5 });

	easing = new Easing();
	easing->Initialize();

	effects = new Effects();
	effects->Initialize(dxCommon->GetDev(), dxCommon->GetCmdQueue(), camera);
	effects->Load(L"effectsTest/10/boneEfk.efk");
}

void Player::Init()
{
	time = 0;
	defence_direction = Previous;
	distance = { 0,0,0 };

	otomoAngle = 0;
	HP = 3;
	isKnock = false;
	isWalk = false;
	rote = 0;
	AnimationTime = 0;
	position = { 0,0,0 };
	isDeath = false;
	deathAnime = false;
}

void Player::ResourceUpdate()
{
	objPlayer->Update();
	fbxPlayer_Run->Update();
	fbxPlayer_Damage->Update();
	fbxPlayer_Wait->Update();
	fbxPlayer_Death->Update();
	fbxPlayer_Attack->Update();
}

void Player::Update(DirectXCommon* dxCommon, Camera* camera)
{
	effects->Play();
	effects->SetPosition({ 0,0,0 });
	effects->SetScale({ 0.3,0.3,0.3 });
	effects->SetSpeed(5);
	effects->Update(dxCommon->GetCmdList(), camera);
	if (isWalk)
	{
		transform = fbxPlayer_Run->GetMatNowPose(fbxPlayer_Attack->GetBoneName("Bip01_R_Hand"));
	}
	else if (AnimetionKnock)
	{
		transform = fbxPlayer_Damage->GetMatNowPose(fbxPlayer_Attack->GetBoneName("Bip01_R_Hand"));
	}
	else if (AnimetionAttack)
	{
		transform = fbxPlayer_Attack->GetMatNowPose(fbxPlayer_Attack->GetBoneName("Bip01_R_Hand"));
	}
	else
	{
		transform = fbxPlayer_Wait->GetMatNowPose(fbxPlayer_Attack->GetBoneName("Bip01_R_Hand"));
	}

	
	matWorld = fbxPlayer_Attack->GetMatWorld();

	Move(camera);
	Jump();
	defense();
	knockBack(dxCommon, camera);
	Death();
	Attack();
	
	effects->Play();
	/*effects->SetPosition({ 0,0,0 });
	effects->SetScale({ 0.3,0.3,0.3 });
	effects->SetSpeed(5);*/
	effects->Update(dxCommon->GetCmdList(), camera);
}

void Player::Move(Camera* camera)
{
	XMVECTOR speedZ = {0,0,0.1,0};//z
	XMVECTOR speedX = { 0.1,0,0,0 };//x
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(cameraAngle));//y軸を中心に回転するマトリックスを作成
	speedZ = XMVector3TransformNormal(speedZ, matRot);
	speedX = XMVector3TransformNormal(speedX, matRot);

	isWalk = false;

	if (AnimetionKnock == false && deathAnime == false)
	{
		if (Input::GetInstance()->PushKey(DIK_W) || Input::GetInstance()->StickTilt(Input::Stick_Up)) {
			position.z += speedZ.m128_f32[2];
			position.x += speedZ.m128_f32[0];
			//camera->MoveVector({ 0,0,forvardvec.m128_f32[2] });
			rote = 0;
			isWalk = true;
			defence_direction = Previous;

		}
		if (Input::GetInstance()->PushKey(DIK_S) || Input::GetInstance()->StickTilt(Input::Stick_Down)) {
			position.z -= speedZ.m128_f32[2];
			position.x -= speedZ.m128_f32[0];
			//camera->MoveVector({ 0,0,-forvardvec.m128_f32[2] });
			rote = 180;
			isWalk = true;
			defence_direction = Back;
		}
		if (Input::GetInstance()->PushKey(DIK_A) || Input::GetInstance()->StickTilt(Input::Stick_Left)) {
			position.x -= speedX.m128_f32[0];
			position.z -= speedX.m128_f32[2];
			//camera->MoveVector({ -forvardvec.m128_f32[0],0,0 });
			rote = -90;
			isWalk = true;
			defence_direction = Left;
		}
		if (Input::GetInstance()->PushKey(DIK_D) || Input::GetInstance()->StickTilt(Input::Stick_Right)) {
			position.x += speedX.m128_f32[0];
			position.z += speedX.m128_f32[2];
			//camera->MoveVector({ forvardvec.m128_f32[0],0,0 });
			rote = 90;
			isWalk = true;
			defence_direction = Right;
		}

		if (isWalk)
		{
			fbxPlayer_Run->SetRotation({ 0,rote + cameraAngle,0 });
			fbxPlayer_Damage->SetRotation({ 0,rote + cameraAngle,0 });
			fbxPlayer_Wait->SetRotation({ 0,rote + cameraAngle,0 });
			fbxPlayer_Death->SetRotation({ 0,rote + cameraAngle,0 });
			fbxPlayer_Attack->SetRotation({ 0,rote + cameraAngle,0 });
		}
	}
	
	fbxPlayer_Run->SetPosition(position);
	fbxPlayer_Damage->SetPosition(position);
	fbxPlayer_Wait->SetPosition(position);
	fbxPlayer_Death->SetPosition(position);
	fbxPlayer_Attack->SetPosition(position);
}

void Player::defenseKey()
{
	XMVECTOR speedZ = { 0,0,5,0 };//z
	XMVECTOR speedX = { 5,0,0,0 };//x
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(cameraAngle));//y軸を中心に回転するマトリックスを作成
	speedZ = XMVector3TransformNormal(speedZ, matRot);
	speedX = XMVector3TransformNormal(speedX, matRot);

	if (defence_direction == Previous)
	{
		endPos.x = oldPos.x + speedZ.m128_f32[0];
		endPos.z = oldPos.z + speedZ.m128_f32[2];
	}
	else if (defence_direction == Back)
	{
		endPos.x = oldPos.x - speedZ.m128_f32[0];
		endPos.z = oldPos.z - speedZ.m128_f32[2];
	}
	else if (defence_direction == Left)
	{
		endPos.x = oldPos.x - speedX.m128_f32[0];
		endPos.z = oldPos.z - speedX.m128_f32[2];
	}
	else if (defence_direction == Right)
	{
		endPos.x = oldPos.x + speedX.m128_f32[0];
		endPos.z = oldPos.z + speedX.m128_f32[2];
	}
	//endPos = { oldPos.x + distance.x, oldPos.y + distance.y, oldPos.z + distance.z };
	defenseMove(endPos);
}

void Player::defense()
{
	if (Input::GetInstance()->TriggerKey(DIK_1) && defenceFlag == false && AnimetionKnock == false)
	{
		time = 0;
		distance = { 0,0,0 };
		defenceFlag = true;
		oldPos = objPlayer->GetPosition();
	}


	if (defenceFlag)
	{
		defenseKey();

	}


	objPlayer->SetPosition(position);
	fbxPlayer_Run->SetPosition(position);
	fbxPlayer_Damage->SetPosition(position);
	fbxPlayer_Wait->SetPosition(position);
	fbxPlayer_Death->SetPosition(position);
	fbxPlayer_Attack->SetPosition(position);
}

void Player::defenseMove(XMFLOAT3 FinalPos)
{
	if (time >= 0 && time <= easing->maxflame)
	{
		time += 0.1f;
		position = easing->ease(oldPos, FinalPos, time);
	}

	if (time >= easing->maxflame)
	{
		distance = { 0,0,0 };
		storagePos = position;
		defenceFlag = false;
	}
}

void Player::Jump()
{
	if (Input::GetInstance()->TriggerKey(DIK_2) && jumpFlag == false && AnimetionKnock == false)
	{
		jumpFlag = true;
	}

	if (jumpFlag == true)
	{
		position.y += gravity / 60.0f;
		if (position.y >= 4)
		{
			gravityFlag = true;
		}
	}

	if (gravityFlag == true)
	{
		jumpFlag = false;
		position.y -= gravity / 60.0f;

		if (position.y < 0)
		{
			gravityFlag = false;
		}
	}

	objPlayer->SetPosition(position);
	fbxPlayer_Run->SetPosition(position);
	fbxPlayer_Damage->SetPosition(position);
	fbxPlayer_Wait->SetPosition(position);
	fbxPlayer_Death->SetPosition(position);
	fbxPlayer_Attack->SetPosition(position);
}

void Player::knockBack(DirectXCommon* dxCommon, Camera* camera)
{
	XMVECTOR v0_Player = { 0, 0, 0.5f };
	//angleラジアンだけy軸まわりに回転。半径は-100
	XMMATRIX rotM_Player = DirectX::XMMatrixIdentity();
	rotM_Player *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(cameraAngle));
	XMVECTOR v = XMVector3TransformNormal(v0_Player, rotM_Player);
	XMVECTOR playerVec = { position.x, position.y, position.z };
	XMVECTOR playerPos_V = { playerVec.m128_f32[0] - v.m128_f32[0], playerVec.m128_f32[1] + v.m128_f32[1], playerVec.m128_f32[2] - v.m128_f32[2] };
	XMFLOAT3 playerPos = { playerPos_V.m128_f32[0], playerPos_V.m128_f32[1], playerPos_V.m128_f32[2] };

	knock_OldPos = position;
	knock_EndPos = playerPos;
	//isKnock = true;
	
	if (isKnock && HP > 0)
	{
		/*effects->Play();
		effects->Update(dxCommon->GetCmdList(), camera);*/
		knockTime += 0.1f;
		position = easing->ease(knock_OldPos, knock_EndPos, knockTime);
		AnimetionKnock = true;
		

		if (knockTime >= easing->maxflame)
		{
			knockTime = 0;
			isKnock = false;
			
			
		}
	}

	if (AnimetionKnock)
	{
		AnimationTime = fbxPlayer_Damage->GetNowTime() + (fbxPlayer_Damage->GetFrame() * 2);
		if (AnimationTime >= fbxPlayer_Damage->GetEndTime())
		{
				AnimetionKnock = false;
				fbxPlayer_Damage->SetNowTime(fbxPlayer_Damage->GetStartTime());
				AnimationTime = fbxPlayer_Damage->GetStartTime();
		}
	}
	else
	{
		AnimationTime = fbxPlayer_Damage->GetStartTime();
		fbxPlayer_Damage->SetNowTime(fbxPlayer_Damage->GetStartTime());
	}

	fbxPlayer_Run->SetPosition(position);
	fbxPlayer_Damage->SetPosition(position);
	fbxPlayer_Wait->SetPosition(position);
	fbxPlayer_Death->SetPosition(position);
	fbxPlayer_Attack->SetPosition(position);
}

void Player::Attack()
{
	if (Input::GetInstance()->TriggerKey(DIK_L) && isKnock == false)
	{
		AnimetionAttack = true;
	}
	if (AnimetionAttack)
	{
		attackAnimeTime = fbxPlayer_Attack->GetNowTime() + (fbxPlayer_Attack->GetFrame() * 2);
		if (attackAnimeTime >= fbxPlayer_Attack->GetEndTime())
		{
			AnimetionAttack = false;
			fbxPlayer_Attack->SetNowTime(fbxPlayer_Attack->GetStartTime());
			attackAnimeTime = fbxPlayer_Attack->GetStartTime();
		}
	}
	else
	{
		attackAnimeTime = fbxPlayer_Attack->GetStartTime();
		fbxPlayer_Attack->SetNowTime(fbxPlayer_Attack->GetStartTime());
	}
}

void Player::Death()
{
	if (HP == 0)
	{
		deathAnime = true;
		
	}

	if (deathAnime)
	{
		deathAnimationTime = fbxPlayer_Death->GetNowTime() + fbxPlayer_Death->GetFrame();
		if (deathAnimationTime >= fbxPlayer_Death->GetEndTime())
		{
			isDeath = true;
			deathAnime = false;
			fbxPlayer_Death->SetNowTime(fbxPlayer_Death->GetStartTime());
			deathAnimationTime = fbxPlayer_Death->GetStartTime();
		}
	}
	else
	{
		deathAnimationTime = fbxPlayer_Death->GetStartTime();
		fbxPlayer_Death->SetNowTime(fbxPlayer_Death->GetStartTime());
	}
}

void Player::Draw(DirectXCommon* dxCommon)
{
	//objPlayer->Draw();
	
	if (isWalk)
	{
		fbxPlayer_Run->Draw(dxCommon->GetCmdList());
	}
	else if (AnimetionKnock)
	{
		fbxPlayer_Damage->Draw(dxCommon->GetCmdList());
	}
	else if (deathAnime)
	{
		fbxPlayer_Death->Draw(dxCommon->GetCmdList());
	}
	else if (AnimetionAttack)
	{
		fbxPlayer_Attack->Draw(dxCommon->GetCmdList());
	}
	else
	{
		fbxPlayer_Wait->Draw(dxCommon->GetCmdList());
	}
	
	
	
	effects->Draw(dxCommon->GetCmdList());

}


