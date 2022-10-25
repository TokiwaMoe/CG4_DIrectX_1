#include "Effects.h"

void Effects::Initialize(DirectXCommon* dxCommon)
{
	DXGI_FORMAT bbFormats[] = { DXGI_FORMAT_R8G8B8A8_UNORM };
	_efkRenderer = EffekseerRendererDX12::Create(
		dxCommon->GetDev(),	//DirectX12のデバイス
		dxCommon->GetCmdQueue(),	//DirectX12のコマンドキュー
		2,	//バックバッファーの数
		bbFormats,	//レンダーターゲットフォーマット
		1,	//レンダーターゲット数
		DXGI_FORMAT_UNKNOWN,	//デプスフォーマット
		false,	//デプスがあるか
		10000);	//最大パーティクルの数

	//マネージャー初期化
	_efkManager = Effekseer::Manager::Create(10000);	//最大インスタンス数
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
	_effect = Effekseer::Effect::Create(
		_efkManager,
		(const EFK_CHAR*)L"effectsTest/10/SimpleLaser.efk",	//エフェクトファイル
		1.0f,	//スケーリング値
		(const EFK_CHAR*)L"effectsTest/10"	//エフェクト基準フォルダー(テクスチャを探すのに必要)
	);

	//エフェクトの再生
	_efkHandle = _efkManager->Play(_effect, 0, 0, 0);
}

void Effects::Update()
{

}
