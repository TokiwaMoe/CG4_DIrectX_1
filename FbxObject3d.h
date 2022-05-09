#pragma once
#include"FbxModel.h"
#include"Camera.h"

#include<Windows.h>
#include<wrl.h>
#include<d3d12.h>
#include<d3dx12.h>
#include<DirectXMath.h>
#include<string>

class FbxObject3d
{
protected://エイリアス
	//Microsoft::WRL::を省略
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public://サブクラス
	//定数バッファ用データ構造体(座標変換行列用)
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj;		//ビュープロジェクション行列
		XMMATRIX world;			//ワールド行列
		XMFLOAT3 cameraPos;		//カメラ座標(ワールド座標)
	};

public://静的メンバ関数
	//setter
	static void SetDevice(ID3D12Device* device) { FbxObject3d::device = device; }
	static void SetCamera(Camera* camera) { FbxObject3d::camera = camera; }

public://メンバ変数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// グラフィックパイプラインの生成
	/// </summary>
	static void CreateGraphicsPipline();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// モデルのセット
	/// </summary>
	/// <param name="model">モデル</param>
	void SetModel(FbxModel* model) { this->model = model; }

protected://メンバ変数
	//定数バッファ
	ComPtr<ID3D12Resource>  constBuffertransform;
	//ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	//パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;

private://静的メンバ変数
	//デバイス
	static ID3D12Device* device;
	//カメラ
	static Camera* camera;

protected://メンバ変数
	//ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };
	//X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation = { 0,0,0 };
	//ローカル座標
	XMFLOAT3 position = { 0,0,0 };
	//ローカルワールド変換行列
	XMMATRIX matWorld;
	//モデル
	FbxModel* model = nullptr;
};
