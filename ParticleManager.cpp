﻿#include "ParticleManager.h"
#include <d3dcompiler.h>
#include <DirectXTex.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
const float ParticleManager::radius = 5.0f;				// 底面の半径
const float ParticleManager::prizmHeight = 8.0f;			// 柱の高さ
ID3D12Device* ParticleManager::device = nullptr;
UINT ParticleManager::descriptorHandleIncrementSize = 0;
ID3D12GraphicsCommandList* ParticleManager::cmdList = nullptr;
ComPtr<ID3D12RootSignature> ParticleManager::rootsignature;
ComPtr<ID3D12PipelineState> ParticleManager::pipelinestate;
ComPtr<ID3D12DescriptorHeap> ParticleManager::descHeap;
ComPtr<ID3D12Resource> ParticleManager::vertBuff;
//ComPtr<ID3D12Resource> ParticleManager::indexBuff;
ComPtr<ID3D12Resource> ParticleManager::texbuff;
CD3DX12_CPU_DESCRIPTOR_HANDLE ParticleManager::cpuDescHandleSRV;
CD3DX12_GPU_DESCRIPTOR_HANDLE ParticleManager::gpuDescHandleSRV;
XMMATRIX ParticleManager::matView{};
XMMATRIX ParticleManager::matProjection{};
XMFLOAT3 ParticleManager::eye = { 0, 0, -5.0f };
XMFLOAT3 ParticleManager::target = { 0, 0, 0 };
XMFLOAT3 ParticleManager::up = { 0, 1, 0 };
D3D12_VERTEX_BUFFER_VIEW ParticleManager::vbView{};
//D3D12_INDEX_BUFFER_VIEW ParticleManager::ibView{};
ParticleManager::VertexPos ParticleManager::vertices[vertexCount];
//unsigned short ParticleManager::indices[planeCount * 3];
//unsigned short ParticleManager::indices[indexCount];
XMMATRIX ParticleManager::matBillboard = XMMatrixIdentity();
XMMATRIX ParticleManager::matBillboadY = XMMatrixIdentity();

const DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& lhs, const DirectX::XMFLOAT3& rhs)
{
	XMFLOAT3 result;
	result.x = lhs.x + rhs.x;
	result.y = lhs.y + rhs.y;
	result.z = lhs.z + rhs.z;
	return result;
}

void ParticleManager::StaticInitialize(ID3D12Device * device, int window_width, int window_height)
{
	// nullptrチェック
	assert(device);

	ParticleManager::device = device;
		
	// デスクリプタヒープの初期化
	InitializeDescriptorHeap();

	// カメラ初期化
	InitializeCamera(window_width, window_height);

	// パイプライン初期化
	InitializeGraphicsPipeline();

	// テクスチャ読み込み
	LoadTexture();

	// モデル生成
	CreateModel();

	// nullptrチェック
	assert(device);

	HRESULT result;
	// 定数バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
}

void ParticleManager::PreDraw(ID3D12GraphicsCommandList * cmdList)
{
	// PreDrawとPostDrawがペアで呼ばれていなければエラー
	assert(ParticleManager::cmdList == nullptr);

	// コマンドリストをセット
	ParticleManager::cmdList = cmdList;

	// パイプラインステートの設定
	cmdList->SetPipelineState(pipelinestate.Get());
	// ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(rootsignature.Get());
	// プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
}

void ParticleManager::PostDraw()
{
	// コマンドリストを解除
	ParticleManager::cmdList = nullptr;
}

ParticleManager * ParticleManager::Create(ID3D12Device* device, int window_width, int window_height)
{
	// 3Dオブジェクトのインスタンスを生成
	ParticleManager* particleMan = new ParticleManager();
	if (particleMan == nullptr) {
		return nullptr;
	}

	// 初期化
	particleMan->StaticInitialize(device, window_width, window_height);

	return particleMan;
}

void ParticleManager::SetEye(XMFLOAT3 eye)
{
	ParticleManager::eye = eye;

	UpdateViewMatrix();
}

void ParticleManager::SetTarget(XMFLOAT3 target)
{
	ParticleManager::target = target;

	UpdateViewMatrix();
}

void ParticleManager::CameraMoveVector(XMFLOAT3 move)
{
	XMFLOAT3 eye_moved = GetEye();
	XMFLOAT3 target_moved = GetTarget();

	eye_moved.x += move.x;
	eye_moved.y += move.y;
	eye_moved.z += move.z;

	target_moved.x += move.x;
	target_moved.y += move.y;
	target_moved.z += move.z;

	SetEye(eye_moved);
	SetTarget(target_moved);
}

void ParticleManager::Add(int life, XMFLOAT3 position, XMFLOAT3 velocity, XMFLOAT3 accel, float start_scale, float end_scale)
{
	//リストに要素を追加
	particles.emplace_front();
	//追加した要素の参照
	Particle& p = particles.front();
	//値のセット
	p.position = position;
	p.velocity = velocity;
	p.accel = accel;
	p.num_frame = life;
	p.s_scale = start_scale;
	p.e_scale = end_scale;
	p.scale = p.s_scale;
}

bool ParticleManager::InitializeDescriptorHeap()
{
	HRESULT result = S_FALSE;
	
	// デスクリプタヒープを生成	
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	descHeapDesc.NumDescriptors = 1; // シェーダーリソースビュー1つ
	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap));//生成
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	// デスクリプタサイズを取得
	descriptorHandleIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	return true;
}

void ParticleManager::InitializeCamera(int window_width, int window_height)
{
	// ビュー行列の生成
	matView = XMMatrixLookAtLH(
		XMLoadFloat3(&eye),
		XMLoadFloat3(&target),
		XMLoadFloat3(&up));

	// 平行投影による射影行列の生成
	//constMap->mat = XMMatrixOrthographicOffCenterLH(
	//	0, window_width,
	//	window_height, 0,
	//	0, 1);
	// 透視投影による射影行列の生成
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		(float)window_width / window_height,
		0.1f, 1000.0f
	);

	//ビュー行列の計算
	UpdateViewMatrix();

}

bool ParticleManager::InitializeGraphicsPipeline()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> gsBlob;	//ジオメトリシェーダーオブジェクト
	ComPtr<ID3DBlob> psBlob;	// ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resource/shaders/ParticleVS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ジオメトリシェーダーの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resource/shaders/ParticleGS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "gs_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&gsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resource/shaders/ParticlePS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy座標(1行で書いたほうが見やすい)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{//スケール
			"TEXCOORD", 0, DXGI_FORMAT_R32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		}
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.GS = CD3DX12_SHADER_BYTECODE(gsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;
	/*blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;*/
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_ONE;
	blenddesc.DestBlend = D3D12_BLEND_ONE;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	gpipeline.NumRenderTargets = 1;	// 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	//デプスの書き込みを禁止
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[2];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ルートシグネチャの生成
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));
	if (FAILED(result)) {
		return result;
	}

	gpipeline.pRootSignature = rootsignature.Get();

	// グラフィックスパイプラインの生成
	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate));

	if (FAILED(result)) {
		return result;
	}

	return true;
}

bool ParticleManager::LoadTexture()
{
	HRESULT result = S_FALSE;

	// WICテクスチャのロード
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	result = LoadFromWICFile(
		L"Resource/alpha.png", WIC_FLAGS_NONE,
		&metadata, scratchImg);
	if (FAILED(result)) {
		return result;
	}

	const Image* img = scratchImg.GetImage(0, 0, 0); // 生データ抽出

	// リソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels
	);

	// テクスチャ用バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // テクスチャ用指定
		nullptr,
		IID_PPV_ARGS(&texbuff));
	if (FAILED(result)) {
		return result;
	}

	// テクスチャバッファにデータ転送
	result = texbuff->WriteToSubresource(
		0,
		nullptr, // 全領域へコピー
		img->pixels,    // 元データアドレス
		(UINT)img->rowPitch,  // 1ラインサイズ
		(UINT)img->slicePitch // 1枚サイズ
	);
	if (FAILED(result)) {
		return result;
	}

	// シェーダリソースビュー作成
	cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);
	gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
	D3D12_RESOURCE_DESC resDesc = texbuff->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView(texbuff.Get(), //ビューと関連付けるバッファ
		&srvDesc, //テクスチャ設定情報
		cpuDescHandleSRV
	);

	return true;
}

void ParticleManager::CreateModel()
{
	HRESULT result = S_FALSE;

	//四角形の頂点データ
	//VertexPos verticesPoint[] = {
	//	//{{0.0f,0.0f,0.0f},{0,0,1},{0,1}},
	//	{{0.0f,0.0f,0.0f}}
	//};
	////メンバ変数にコピー
	//std::copy(std::begin(verticesPoint), std::end(verticesPoint), vertices);

	/*for (int i = 0; i < vertexCount; i++)
	{
		const float md_width = 10.0f;
		vertices[i].pos.x = (float)rand() / RAND_MAX * md_width - md_width / 2.0f;
		vertices[i].pos.y = (float)rand() / RAND_MAX * md_width - md_width / 2.0f;
		vertices[i].pos.z = (float)rand() / RAND_MAX * md_width - md_width / 2.0f;
	}*/
	//四角形のインデックスデータ
	//unsigned short indicesSquare[] = {
	//	0,1,2,//三角形1
	//	2,1,3//三角形2
	//};
	//メンバ変数にコピー
	//std::copy(std::begin(indicesSquare), std::end(indicesSquare), indices);
	//std::vector<VertexPosNormalUv> realVertices;
	//// 頂点座標の計算（重複あり）
	//{
	//	realVertices.resize((division + 1) * 2);
	//	int index = 0;
	//	float zValue;

	//	// 底面
	//	zValue = prizmHeight / 2.0f;
	//	for (int i = 0; i < division; i++)
	//	{
	//		XMFLOAT3 vertex;
	//		vertex.x = radius * sinf(XM_2PI / division * i);
	//		vertex.y = radius * cosf(XM_2PI / division * i);
	//		vertex.z = zValue;
	//		realVertices[index++].pos = vertex;
	//	}
	//	realVertices[index++].pos = XMFLOAT3(0, 0, zValue);	// 底面の中心点
	//	// 天面
	//	zValue = -prizmHeight / 2.0f;
	//	for (int i = 0; i < division; i++)
	//	{
	//		XMFLOAT3 vertex;
	//		vertex.x = radius * sinf(XM_2PI / division * i);
	//		vertex.y = radius * cosf(XM_2PI / division * i);
	//		vertex.z = zValue;
	//		realVertices[index++].pos = vertex;
	//	}
	//	realVertices[index++].pos = XMFLOAT3(0, 0, zValue);	// 天面の中心点
	//}

	//// 頂点座標の計算（重複なし）
	//{
	//	int index = 0;
	//	// 底面
	//	for (int i = 0; i < division; i++)
	//	{
	//		unsigned short index0 = i + 1;
	//		unsigned short index1 = i;
	//		unsigned short index2 = division;

	//		vertices[index++] = realVertices[index0];
	//		vertices[index++] = realVertices[index1];
	//		vertices[index++] = realVertices[index2]; // 底面の中心点
	//	}
	//	// 底面の最後の三角形の1番目のインデックスを0に書き換え
	//	vertices[index - 3] = realVertices[0];

	//	int topStart = division + 1;
	//	// 天面
	//	for (int i = 0; i < division; i++)
	//	{
	//		unsigned short index0 = topStart + i;
	//		unsigned short index1 = topStart + i + 1;
	//		unsigned short index2 = topStart + division;

	//		vertices[index++] = realVertices[index0];
	//		vertices[index++] = realVertices[index1];
	//		vertices[index++] = realVertices[index2]; // 天面の中心点
	//	}
	//	// 天面の最後の三角形の1番目のインデックスを0に書き換え
	//	vertices[index - 2] = realVertices[topStart];

	//	// 側面
	//	for (int i = 0; i < division; i++)
	//	{
	//		unsigned short index0 = i + 1;
	//		unsigned short index1 = topStart + i + 1;
	//		unsigned short index2 = i;
	//		unsigned short index3 = topStart + i;

	//		if (i == division - 1)
	//		{
	//			index0 = 0;
	//			index1 = topStart;
	//		}

	//		vertices[index++] = realVertices[index0];
	//		vertices[index++] = realVertices[index1];
	//		vertices[index++] = realVertices[index2];

	//		vertices[index++] = realVertices[index2];
	//		vertices[index++] = realVertices[index1];
	//		vertices[index++] = realVertices[index3];
	//	}
	//}

	//// 頂点インデックスの設定
	//{
	//	for (int i = 0; i < _countof(indices); i++)
	//	{
	//		indices[i] = i;
	//	}
	//}

	//// 法線方向の計算
	//for (int i = 0; i < _countof(indices) / 3; i++)
	//{// 三角形１つごとに計算していく
	//	// 三角形のインデックスを取得
	//	unsigned short index0 = indices[i * 3 + 0];
	//	unsigned short index1 = indices[i * 3 + 1];
	//	unsigned short index2 = indices[i * 3 + 2];
	//	// 三角形を構成する頂点座標をベクトルに代入
	//	XMVECTOR p0 = XMLoadFloat3(&vertices[index0].pos);
	//	XMVECTOR p1 = XMLoadFloat3(&vertices[index1].pos);
	//	XMVECTOR p2 = XMLoadFloat3(&vertices[index2].pos);
	//	// p0→p1ベクトル、p0→p2ベクトルを計算
	//	XMVECTOR v1 = XMVectorSubtract(p1, p0);
	//	XMVECTOR v2 = XMVectorSubtract(p2, p0);
	//	// 外積は両方から垂直なベクトル
	//	XMVECTOR normal = XMVector3Cross(v1, v2);
	//	// 正規化（長さを1にする)
	//	normal = XMVector3Normalize(normal);
	//	// 求めた法線を頂点データに代入
	//	XMStoreFloat3(&vertices[index0].normal, normal);
	//	XMStoreFloat3(&vertices[index1].normal, normal);
	//	XMStoreFloat3(&vertices[index2].normal, normal);
	//}

	// 頂点バッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	if (FAILED(result)) {
		assert(0);
		return;
	}

	// 頂点バッファへのデータ転送
	VertexPos* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(vertices);
	vbView.StrideInBytes = sizeof(vertices[0]);
}

void ParticleManager::UpdateViewMatrix()
{
	// ビュー行列の更新
	//matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
	
	//視点座標
	XMVECTOR eyePosition = XMLoadFloat3(&eye);
	//注視点座標
	XMVECTOR targetPosition = XMLoadFloat3(&target);
	//(仮の)上方向
	XMVECTOR upVector = XMLoadFloat3(&up);
	//カメラZ軸
	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPosition, eyePosition);

	//0ベクトルだと向きが定まらないので除外
	assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxisZ));
	assert(!XMVector3Equal(upVector, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVector));

	//ベクトルを正規化
	cameraAxisZ = XMVector3Normalize(cameraAxisZ);

	//カメラのX軸(右方向)
	XMVECTOR cameraAxisX;
	//X軸は上方向→Z軸の外積で求まる
	cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);
	//ベクトルを正規化
	cameraAxisX = XMVector3Normalize(cameraAxisX);
	//カメラのY軸(上方向)
	XMVECTOR cameraAxisY;
	//Y軸はZ軸→X軸の外積で求まる
	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);
	//カメラ回転行列
	XMMATRIX matCameraRot;
	//カメラ座標系→ワールド座標系の変換行列
	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);
	//転置により逆行列(逆回転)を計算
	matView = XMMatrixTranspose(matCameraRot);
	//視点座標に-1を掛けた座標
	XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
	//カメラの位置からワールド原点へのベクトル(カメラ座標系)
	XMVECTOR tX = XMVector3Dot(cameraAxisX, reverseEyePosition);	//X成分
	XMVECTOR tY = XMVector3Dot(cameraAxisY, reverseEyePosition);	//Y成分
	XMVECTOR tZ = XMVector3Dot(cameraAxisZ, reverseEyePosition);	//Z成分
	//一つのベクトルにまとめる
	XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);
	//ビュー行列に平行移動成分を設定
	matView.r[3] = translation;
#pragma region 全方向ビルボード行列の計算
	//ビルボード行列
	matBillboard.r[0] = cameraAxisX;
	matBillboard.r[1] = cameraAxisY;
	matBillboard.r[2] = cameraAxisZ;
	matBillboard.r[3] = XMVectorSet(0,0,0,1);
#pragma region Y軸回りビルボード行列の計算
	//カメラX軸,Y軸,Z軸
	XMVECTOR ybillCameraAxisX, ybillCameraAxisY, ybillCameraAxisZ;

	//X軸は共通
	ybillCameraAxisX = cameraAxisX;
	//Y軸はワールド座標系のY軸
	ybillCameraAxisY = XMVector3Normalize(upVector);
	//Z軸はX軸→Y軸の外積で求まる
	ybillCameraAxisZ = XMVector3Cross(
		cameraAxisZ, cameraAxisY
	);

	//Y軸回りビルボード行列
	matBillboadY.r[0] = ybillCameraAxisX;
	matBillboadY.r[1] = ybillCameraAxisY;
	matBillboadY.r[2] = ybillCameraAxisZ;
	matBillboadY.r[3] = XMVectorSet(0, 0, 0, 1);
#pragma endregion
}

void ParticleManager::Update()
{
	HRESULT result;
	//寿命が尽きたパーティクルを全削除
	particles.remove_if(
		[](Particle& x) {
			return x.frame >= x.num_frame;
		}
	);
	//全パーティクル更新
	for (std::forward_list<Particle>::iterator it = particles.begin();
		it != particles.end(); it++)
	{
		//経過フレーム数をカウント
		it->frame++;
		//速度に加速度を加算
		it->velocity = it->velocity + it->accel;
		//速度による移動
		it->position = it->position + it->velocity;

		//進行度を0～1の範囲に換算
		float f = (float)it->num_frame / it->frame;
		//スケールの線形補間
		it->scale = (it->e_scale - it->s_scale) / f;
		it->scale += it->s_scale;
	}
	//頂点バッファへデータ転送
	VertexPos* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result))
	{
		for (std::forward_list<Particle>::iterator it = particles.begin();
			it != particles.end(); it++)
		{
			//座標6
			vertMap->pos = it->position;
			//スケール
			vertMap->scale = it->scale;
			//次の頂点へ
			vertMap++;
		}
		vertBuff->Unmap(0, nullptr);
	}
	// 定数バッファへデータ転送
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = matView * matProjection;	// 行列の合成
	constMap->matBillboard = matBillboard;
	constBuff->Unmap(0, nullptr);
}

void ParticleManager::Draw(ID3D12GraphicsCommandList* cmdList)
{
	// nullptrチェック
	assert(device);
		
	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	// インデックスバッファの設定
	//cmdList->IASetIndexBuffer(&ibView);

	// デスクリプタヒープの配列
	ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	// シェーダリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(1, gpuDescHandleSRV);
	// 描画コマンド
	//cmdList->DrawInstanced(_countof(vertices), 1, 0, 0);
	cmdList->DrawInstanced((UINT)std::distance(particles.begin(), particles.end()), 1, 0, 0);
}

void ParticleManager::CameraMoveEyeVector(XMFLOAT3 move)
{
	XMFLOAT3 eye_moved = GetEye();

	eye_moved.x += move.x;
	eye_moved.y += move.y;
	eye_moved.z += move.z;

	SetEye(eye_moved);
}
