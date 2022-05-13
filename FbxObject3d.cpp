#include"FbxObject3d.h"
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace DirectX;

///<summary>
///静的メンバ変数の実体
/// </summary>
ID3D12Device* FbxObject3d::device = nullptr;
Camera* FbxObject3d::camera = nullptr;
ComPtr<ID3D12RootSignature> FbxObject3d::rootsignature;
ComPtr<ID3D12PipelineState> FbxObject3d::pipelinestate;

void FbxObject3d::Initialize()
{
	HRESULT result;
	//定数バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataTransform) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform));
}

void FbxObject3d::CreateGraphicsPipline()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;	// ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	//頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resource/shaders/FBXVS.hlsl",	//シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vsBlob, &errorBlob);

	//デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	//ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[2];
	//CBV(座標変換行列用)
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	//SRV(テクスチャ)
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
}

void FbxObject3d::Update()
{
	XMMATRIX matScale, matRot, matTrans;

	//スケール、 回転、 平行移動行列の計算
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	//ワールド行列の合成
	matWorld = XMMatrixIdentity();//変換をリセット
	matWorld *= matScale;//ワールド行列にスケーリングを反映
	matWorld *= matRot;//ワールド行列に回転を反映
	matWorld *= matTrans;//ワールド行列に平行移動を反映

	//ビュープロジェクション
	const XMMATRIX& matViewProjection = camera->GetViewProjectionMatrix();
	//モデルのメッシュトランスフォーム
	const XMMATRIX& modelTransform = model->GetModelTransform();
	//カメラ座標
	const XMFLOAT3& cameraPos = camera->GetEye();

	HRESULT result;
	//定数バッファへデータ転送
	ConstBufferDataTransform* constMap = nullptr;
	result = constBuffTransform->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->viewproj = matViewProjection;
		constMap->world = modelTransform * matWorld;
		constMap->cameraPos = cameraPos;
		constBuffTransform->Unmap(0, nullptr);
	}
}

void FbxObject3d::Draw(ID3D12GraphicsCommandList* cmdList)
{
	//モデルの割り当てがなければ描画しない
	if (model == nullptr)
	{
		return;
	}
	//パイプラインステートの設定
	cmdList->SetPipelineState(pipelinestate.Get());
	//ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(rootsignature.Get());
	//プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffTransform->GetGPUVirtualAddress());

	//モデル描画
	model->Draw(cmdList);
}
