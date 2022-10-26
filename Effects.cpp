#include "Effects.h"
Effects::Effects(){}
Effects::~Effects(){}

void Effects::Initialize(ID3D12Device* device, ID3D12CommandQueue* cmdQueue, Camera *camera)
{
	assert(device);
	assert(cmdQueue);

	DXGI_FORMAT bbFormats[] = { DXGI_FORMAT_R8G8B8A8_UNORM };
	_efkRenderer = EffekseerRendererDX12::Create(
		device,	//DirectX12のデバイス
		cmdQueue,	//DirectX12のコマンドキュー
		2,	//バックバッファーの数
		bbFormats,	//レンダーターゲットフォーマット
		1,	//レンダーターゲット数
		DXGI_FORMAT_UNKNOWN,	//デプスフォーマット
		false,	//デプスがあるか
		1000);	//最大パーティクルの数

	

	//マネージャー初期化
	_efkManager = Effekseer::Manager::Create(1000);	//最大インスタンス数
	//座標系を左手系にする(クライアント側の系に合わせる)
	_efkManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);
	//描画用インスタンスから描画機能を設定
	_efkManager->SetSpriteRenderer(_efkRenderer->CreateSpriteRenderer());
	_efkManager->SetRibbonRenderer(_efkRenderer->CreateRibbonRenderer());
	_efkManager->SetRingRenderer(_efkRenderer->CreateRingRenderer());
	_efkManager->SetTrackRenderer(_efkRenderer->CreateTrackRenderer());
	_efkManager->SetModelRenderer(_efkRenderer->CreateModelRenderer());

	//描画用インスタンスからテクスチャの読み込み機能も設定
	//独自拡張も可能
	_efkManager->SetTextureLoader(_efkRenderer->CreateTextureLoader());
	_efkManager->SetModelLoader(_efkRenderer->CreateModelLoader());

	//DirectX12特有の処理
	_efkMemoryPool = EffekseerRenderer::CreateSingleFrameMemoryPool(_efkRenderer->GetGraphicsDevice());
	_efkCmdList = EffekseerRenderer::CreateCommandList(_efkRenderer->GetGraphicsDevice(), _efkMemoryPool);

	_efkRenderer->SetCommandList(_efkCmdList);

	

	//エフェクトの読み込み
	//_effect = Effekseer::Effect::Create(
	//	_efkManager,
	//	(const EFK_CHAR*)L"effectsTest/10/SimpleLaser.efk",	//エフェクトファイル
	//	1.0f,	//スケーリング値
	//	(const EFK_CHAR*)L"effectsTest/10"	//エフェクト基準フォルダー(テクスチャを探すのに必要)
	//);

	_effect1 = Effekseer::Effect::Create(
		_efkManager,
		(const EFK_CHAR*)L"effectsTest/10/firework.efk",	//エフェクトファイル
		1.0f,	//スケーリング値
		(const EFK_CHAR*)L"effectsTest/Textures"	//エフェクト基準フォルダー(テクスチャを探すのに必要)
	);

	
}

void Effects::Play()
{
	_efkHandle1 = _efkManager->Play(_effect1, 3, 0, 0);
}

void Effects::Update(ID3D12GraphicsCommandList *cmdList, Camera *camera)
{
	//エフェクトの再生
	//_efkHandle = _efkManager->Play(_effect1, 0, 0, 0);
	_efkManager->SetScale(_efkHandle, 1, 1, 1);
	_efkManager->SetRotation(_efkHandle1, { 0,0,0 }, 180);
	
	//カメラ設定
	SetCamera(camera);
}

void Effects::Draw(ID3D12GraphicsCommandList* cmdList)
{
	//エフェクト描画
	_efkManager->Update();	//マネージャーの更新(時間更新)
	_efkMemoryPool->NewFrame();	//描画すべきレンダーターゲットを選択
	EffekseerRendererDX12::BeginCommandList(_efkCmdList, cmdList);
	_efkRenderer->BeginRendering();	//描画前処理
	_efkManager->Draw();	//エフェクト描画
	_efkRenderer->EndRendering();	//描画後処理

	EffekseerRendererDX12::EndCommandList(_efkCmdList);
}

void Effects::SetCamera(Camera* camera)
{
	Effekseer::Matrix44 fkViewMat;
	Effekseer::Matrix44 fkProjMat;
	auto view = camera->GetViewMatrix();//カメラのセット
	auto proj = camera->GetProjectionMatrix();//射影、透視投影を代入
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			fkViewMat.Values[i][j] = view.r[i].m128_f32[j];
			fkProjMat.Values[i][j] = proj.r[i].m128_f32[j];
		}
	}
	_efkRenderer->SetCameraMatrix(fkViewMat);
	_efkRenderer->SetProjectionMatrix(fkProjMat);
}
